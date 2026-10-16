#ifndef SENSOR_HUB_MQTT_H
#define SENSOR_HUB_MQTT_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "sensorHubConfig.h"

namespace sensor_hub {

class SensorHubMqtt {
public:
    SensorHubMqtt();
    ~SensorHubMqtt();

    void configure(const HubConfig &config);
    bool begin();
    void stop();
    void loop();

    bool isEnabled() const { return enabled_; }
    bool isConnected() const;
    bool publishJsonMessage(const char *payload);

private:
    bool enabled_ = false;
    bool initialized_ = false;
    unsigned long next_reconnect_ms_ = 0;
    char client_id_[32] = "";
    HubConfig config_;
    WiFiClient wifi_client_;
    mutable PubSubClient client_;

    bool ensureConnection();
};

}  // namespace sensor_hub

#endif
