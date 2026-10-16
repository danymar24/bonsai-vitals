#ifndef SENSOR_HUB_MANAGER_H
#define SENSOR_HUB_MANAGER_H

#include <Arduino.h>
#include <esp_now.h>
#include "sensorHubProtocol.h"

namespace sensor_hub {

class SensorHubManager {
public:
    SensorHubManager();
    ~SensorHubManager();

    bool begin();
    void stop();
    void clear();
    bool broadcastChannelAnnouncement(uint8_t channel);

    bool isDiscoveryEnabled() const { return discovery_enabled_; }
    void setDiscoveryEnabled(bool enabled) { discovery_enabled_ = enabled; }

    uint8_t getTrackedDeviceCount() const { return device_count_; }
    const DiscoveredDevice* getTrackedDevices() const { return devices_; }

    bool syncDevice(const char *device_id);
    bool unsyncDevice(const char *device_id);
    bool getDevice(const char *device_id, DiscoveredDevice &device) const;
    bool getDeviceAt(uint8_t index, DiscoveredDevice &device) const;
    bool getLatestSyncedReading(SensorReading &reading) const;
    bool getLatestKnownReading(SensorReading &reading) const;

private:
    static SensorHubManager *instance_;

    bool initialized_ = false;
    bool discovery_enabled_ = true;
    DiscoveredDevice devices_[kMaxTrackedDevices];
    uint8_t device_count_ = 0;

    static void onEspNowReceive(const esp_now_recv_info_t *recv_info, const uint8_t *incoming_data, int len);
    void handleIncomingPacket(const uint8_t *mac_addr, const uint8_t *incoming_data, int len);
    bool validateAndApplyPacket(const uint8_t *mac_addr, const SensorPacket &packet);

    bool appendOrUpdateDevice(const char *device_id, const SensorReading &reading, bool discovered_only = false);
    bool isKnownDevice(const char *device_id) const;
    uint8_t findDeviceIndex(const char *device_id) const;
    void markAsDiscovered(const char *device_id);

    static void macToString(const uint8_t *mac_addr, char *out, size_t out_size);
};

}  // namespace sensor_hub

#endif
