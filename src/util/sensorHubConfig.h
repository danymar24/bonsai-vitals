#ifndef SENSOR_HUB_CONFIG_H
#define SENSOR_HUB_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

namespace sensor_hub {

struct MqttConfig {
    bool enabled = false;
    char broker_host[64] = "";
    uint16_t broker_port = 1883;
    char username[32] = "";
    char password[32] = "";
    char topic[64] = "sensorhub/telemetry";
    bool use_tls = false;
};

struct DiscoveryConfig {
    bool enabled = true;
    uint8_t max_tracked_devices = 8;
    uint32_t stale_after_ms = 300000;
};

struct HubConfig {
    MqttConfig mqtt;
    DiscoveryConfig discovery;
};

void applyDefaultSensorHubConfig(HubConfig &config);
void loadSensorHubConfig(Preferences &prefs, HubConfig &config);
void saveSensorHubConfig(Preferences &prefs, const HubConfig &config);
void clearSensorHubConfig(Preferences &prefs);

}  // namespace sensor_hub

#endif
