#include "sensorHubManager.h"

#include <WiFi.h>
#include <esp_wifi.h>
#include <cstring>
#include <cmath>

namespace sensor_hub {

SensorHubManager *SensorHubManager::instance_ = nullptr;

SensorHubManager::SensorHubManager() {
    memset(devices_, 0, sizeof(devices_));
    instance_ = this;
}

SensorHubManager::~SensorHubManager() {
    stop();
    if (instance_ == this) {
        instance_ = nullptr;
    }
}

bool SensorHubManager::begin() {
    if (initialized_) {
        return true;
    }

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial2.println("ESP-NOW init failed");
        return false;
    }

    esp_now_register_recv_cb(onEspNowReceive);
    initialized_ = true;
    Serial2.println("ESP-NOW initialized for sensor hub");
    return true;
}

void SensorHubManager::stop() {
    if (!initialized_) {
        return;
    }

    esp_now_unregister_recv_cb();
    esp_now_deinit();
    initialized_ = false;
    Serial2.println("ESP-NOW stopped");
}

void SensorHubManager::clear() {
    memset(devices_, 0, sizeof(devices_));
    device_count_ = 0;
}

bool SensorHubManager::syncDevice(const char *device_id) {
    uint8_t index = findDeviceIndex(device_id);
    if (index >= kMaxTrackedDevices) {
        return false;
    }

    devices_[index].is_synced = true;
    devices_[index].synced_at_ms = millis();
    return true;
}

bool SensorHubManager::unsyncDevice(const char *device_id) {
    uint8_t index = findDeviceIndex(device_id);
    if (index >= kMaxTrackedDevices) {
        return false;
    }

    devices_[index].is_synced = false;
    devices_[index].synced_at_ms = 0;
    return true;
}

bool SensorHubManager::getDevice(const char *device_id, DiscoveredDevice &device) const {
    uint8_t index = findDeviceIndex(device_id);
    if (index >= kMaxTrackedDevices) {
        return false;
    }

    device = devices_[index];
    return true;
}

bool SensorHubManager::getDeviceAt(uint8_t index, DiscoveredDevice &device) const {
    if (index >= device_count_) {
        return false;
    }

    device = devices_[index];
    return true;
}

bool SensorHubManager::getLatestSyncedReading(SensorReading &reading) const {
    uint32_t newest_ms = 0;
    bool found = false;

    for (uint8_t i = 0; i < device_count_; ++i) {
        if (!devices_[i].is_synced) {
            continue;
        }

        if (!found || devices_[i].latest.received_at_ms > newest_ms) {
            reading = devices_[i].latest;
            newest_ms = devices_[i].latest.received_at_ms;
            found = true;
        }
    }

    return found;
}

bool SensorHubManager::getLatestKnownReading(SensorReading &reading) const {
    uint32_t newest_ms = 0;
    bool found = false;

    for (uint8_t i = 0; i < device_count_; ++i) {
        if (!found || devices_[i].latest.received_at_ms > newest_ms) {
            reading = devices_[i].latest;
            newest_ms = devices_[i].latest.received_at_ms;
            found = true;
        }
    }

    return found;
}

void SensorHubManager::onEspNowReceive(const esp_now_recv_info_t *recv_info, const uint8_t *incoming_data, int len) {
    if (instance_ == nullptr || recv_info == nullptr || recv_info->src_addr == nullptr) {
        return;
    }

    instance_->handleIncomingPacket(recv_info->src_addr, incoming_data, len);
}

void SensorHubManager::handleIncomingPacket(const uint8_t *mac_addr, const uint8_t *incoming_data, int len) {
    if (!initialized_ || !discovery_enabled_ || incoming_data == nullptr || len < static_cast<int>(sizeof(SensorPacket))) {
        return;
    }

    const SensorPacket *packet = reinterpret_cast<const SensorPacket *>(incoming_data);
    if (memcmp(packet->magic, kPacketMagic, sizeof(packet->magic)) != 0) {
        return;
    }

    if (packet->version != kProtocolVersion) {
        Serial2.println("ESP-NOW packet version mismatch");
        return;
    }

    if (packet->packet_type != static_cast<uint8_t>(PacketType::Discovery) &&
        packet->packet_type != static_cast<uint8_t>(PacketType::SensorData)) {
        return;
    }

    if (!validateAndApplyPacket(mac_addr, *packet)) {
        return;
    }
}

bool SensorHubManager::validateAndApplyPacket(const uint8_t *mac_addr, const SensorPacket &packet) {
    char device_id[kDeviceIdLength];
    macToString(mac_addr, device_id, sizeof(device_id));

    SensorReading reading;
    reading.received_at_ms = millis();
    reading.moisture_percent = packet.moisture_percent;
    reading.temperature_c = packet.temperature_c;
    reading.humidity_percent = packet.humidity_percent;
    reading.aqi = packet.aqi;
    reading.tvoc_ppb = packet.tvoc_ppb;
    reading.eco2_ppm = packet.eco2_ppm;

    if (!std::isfinite(reading.moisture_percent) || reading.moisture_percent < 0.0f || reading.moisture_percent > 100.0f) {
        Serial2.println("ESP-NOW packet rejected: invalid moisture_percent");
        return false;
    }

    if (std::isfinite(reading.temperature_c) && (reading.temperature_c < -50.0f || reading.temperature_c > 100.0f)) {
        reading.temperature_c = NAN;
    }
    if (std::isfinite(reading.humidity_percent) && (reading.humidity_percent < 0.0f || reading.humidity_percent > 100.0f)) {
        reading.humidity_percent = NAN;
    }
    if (std::isfinite(reading.aqi) && reading.aqi < 0.0f) {
        reading.aqi = NAN;
    }
    if (std::isfinite(reading.tvoc_ppb) && reading.tvoc_ppb < 0.0f) {
        reading.tvoc_ppb = NAN;
    }
    if (std::isfinite(reading.eco2_ppm) && reading.eco2_ppm < 0.0f) {
        reading.eco2_ppm = NAN;
    }

    bool added = appendOrUpdateDevice(device_id, reading, packet.packet_type == static_cast<uint8_t>(PacketType::Discovery));
    if (!added) {
        return false;
    }

    uint8_t index = findDeviceIndex(device_id);
    if (index >= kMaxTrackedDevices) {
        return false;
    }

    devices_[index].last_seen_ms = reading.received_at_ms;
    devices_[index].is_discovered = true;
    if (packet.packet_type == static_cast<uint8_t>(PacketType::Discovery)) {
        devices_[index].is_discovered = true;
    }

    return true;
}

bool SensorHubManager::appendOrUpdateDevice(const char *device_id, const SensorReading &reading, bool discovered_only) {
    if (device_id == nullptr || device_id[0] == '\0') {
        return false;
    }

    uint8_t index = findDeviceIndex(device_id);
    if (index < kMaxTrackedDevices) {
        devices_[index].latest = reading;
        devices_[index].last_seen_ms = reading.received_at_ms;
        devices_[index].is_discovered = true;
        if (discovered_only) {
            devices_[index].is_discovered = true;
        }
        return true;
    }

    if (device_count_ >= kMaxTrackedDevices) {
        return false;
    }

    memset(&devices_[device_count_], 0, sizeof(devices_[device_count_]));
    strlcpy(devices_[device_count_].device_id, device_id, sizeof(devices_[device_count_].device_id));
    devices_[device_count_].is_discovered = true;
    devices_[device_count_].latest = reading;
    devices_[device_count_].last_seen_ms = reading.received_at_ms;
    device_count_++;
    return true;
}

bool SensorHubManager::isKnownDevice(const char *device_id) const {
    return findDeviceIndex(device_id) < kMaxTrackedDevices;
}

uint8_t SensorHubManager::findDeviceIndex(const char *device_id) const {
    if (device_id == nullptr || device_id[0] == '\0') {
        return kMaxTrackedDevices;
    }

    for (uint8_t i = 0; i < device_count_; ++i) {
        if (strncmp(devices_[i].device_id, device_id, sizeof(devices_[i].device_id)) == 0) {
            return i;
        }
    }

    return kMaxTrackedDevices;
}

void SensorHubManager::markAsDiscovered(const char *device_id) {
    uint8_t index = findDeviceIndex(device_id);
    if (index < kMaxTrackedDevices) {
        devices_[index].is_discovered = true;
    }
}

void SensorHubManager::macToString(const uint8_t *mac_addr, char *out, size_t out_size) {
    if (out == nullptr || out_size == 0) {
        return;
    }

    snprintf(out, out_size, "%02X:%02X:%02X:%02X:%02X:%02X",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

}  // namespace sensor_hub
