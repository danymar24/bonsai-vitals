#include "sensorHubMqtt.h"

#include <ArduinoJson.h>

namespace sensor_hub {

SensorHubMqtt::SensorHubMqtt() : client_(wifi_client_) {
    snprintf(client_id_, sizeof(client_id_), "sensorhub-%08X", (uint32_t)(ESP.getEfuseMac() & 0xFFFFFFFFULL));
}

SensorHubMqtt::~SensorHubMqtt() {
    stop();
}

void SensorHubMqtt::configure(const HubConfig &config) {
    config_ = config;
    enabled_ = config.mqtt.enabled;

    if (!enabled_) {
        stop();
        return;
    }

    if (config_.mqtt.broker_host[0] == '\0') {
        Serial2.println("MQTT broker host is empty; MQTT disabled");
        enabled_ = false;
        stop();
        return;
    }

    if (!initialized_) {
        begin();
    } else {
        client_.setServer(config_.mqtt.broker_host, config_.mqtt.broker_port);
    }
}

bool SensorHubMqtt::begin() {
    if (!enabled_ || initialized_) {
        return initialized_;
    }

    if (config_.mqtt.broker_host[0] == '\0') {
        Serial2.println("MQTT broker host not configured");
        return false;
    }

    client_.setServer(config_.mqtt.broker_host, config_.mqtt.broker_port);
    initialized_ = true;
    Serial2.printf("MQTT client initialized for broker %s:%u\n", config_.mqtt.broker_host, config_.mqtt.broker_port);

    if (WiFi.status() == WL_CONNECTED) {
        ensureConnection();
    }

    return true;
}

void SensorHubMqtt::stop() {
    if (client_.connected()) {
        client_.disconnect();
    }
    initialized_ = false;
    enabled_ = false;
    next_reconnect_ms_ = 0;
    Serial2.println("MQTT client stopped");
}

void SensorHubMqtt::loop() {
    if (!enabled_ || !initialized_ || WiFi.status() != WL_CONNECTED) {
        return;
    }

    if (client_.connected()) {
        client_.loop();
        return;
    }

    ensureConnection();
}

bool SensorHubMqtt::isConnected() const {
    return enabled_ && initialized_ && client_.connected();
}

bool SensorHubMqtt::publishJsonMessage(const char *payload) {
    if (!enabled_ || !initialized_ || payload == nullptr) {
        return false;
    }

    if (!ensureConnection()) {
        return false;
    }

    const bool published = client_.publish(config_.mqtt.topic, payload, true);
    if (!published) {
        Serial2.println("MQTT publish failed");
    }

    return published;
}

bool SensorHubMqtt::ensureConnection() {
    if (!enabled_ || !initialized_ || WiFi.status() != WL_CONNECTED) {
        return false;
    }

    if (client_.connected()) {
        return true;
    }

    if (millis() < next_reconnect_ms_) {
        return false;
    }

    const bool has_username = config_.mqtt.username[0] != '\0';
    const bool has_password = config_.mqtt.password[0] != '\0';
    bool connected = false;

    if (has_username) {
        connected = client_.connect(client_id_, config_.mqtt.username, has_password ? config_.mqtt.password : "");
    } else {
        connected = client_.connect(client_id_);
    }

    if (connected) {
        Serial2.printf("MQTT connected to %s:%u\n", config_.mqtt.broker_host, config_.mqtt.broker_port);
        next_reconnect_ms_ = 0;
    } else {
        next_reconnect_ms_ = millis() + 15000UL;
        Serial2.printf("MQTT connect failed, retrying in 15s for %s:%u\n", config_.mqtt.broker_host, config_.mqtt.broker_port);
    }

    return connected;
}

}  // namespace sensor_hub
