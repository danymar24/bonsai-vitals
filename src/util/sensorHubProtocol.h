#ifndef SENSOR_HUB_PROTOCOL_H
#define SENSOR_HUB_PROTOCOL_H

#include <Arduino.h>
#include <math.h>

namespace sensor_hub {

static constexpr uint8_t kProtocolVersion = 1;
static constexpr uint8_t kMaxTrackedDevices = 8;
static constexpr uint8_t kDeviceIdLength = 32;
static constexpr uint32_t kDefaultStaleAfterMs = 300000;
static constexpr uint8_t kPacketMagic[4] = {'S', 'H', 'B', '!'};

enum class PacketType : uint8_t {
    Discovery = 1,
    SensorData = 2,
    SyncRequest = 3,
    SyncResponse = 4,
};

struct SensorReading {
    uint32_t received_at_ms = 0;
    float moisture_percent = NAN;
    float temperature_c = NAN;
    float humidity_percent = NAN;
    float aqi = NAN;
    float tvoc_ppb = NAN;
    float eco2_ppm = NAN;

    bool hasRequiredValues() const {
        return !isnan(moisture_percent) && moisture_percent >= 0.0f && moisture_percent <= 100.0f;
    }

    bool hasValue(float value) const {
        return !isnan(value);
    }
};

struct SensorPacket {
    char magic[4] = {'S', 'H', 'B', '!'};
    uint8_t version = kProtocolVersion;
    uint8_t packet_type = 0;
    uint8_t flags = 0;
    uint8_t sender_mac[6] = {0, 0, 0, 0, 0, 0};
    uint32_t sequence = 0;
    uint32_t timestamp_ms = 0;
    float moisture_percent = NAN;
    float temperature_c = NAN;
    float humidity_percent = NAN;
    float aqi = NAN;
    float tvoc_ppb = NAN;
    float eco2_ppm = NAN;
};

struct DiscoveredDevice {
    char device_id[kDeviceIdLength] = "";
    bool is_discovered = false;
    bool is_synced = false;
    uint32_t last_seen_ms = 0;
    uint32_t synced_at_ms = 0;
    SensorReading latest;
};

inline bool isOptionalFieldPresent(float value) {
    return !isnan(value);
}

}  // namespace sensor_hub

#endif
