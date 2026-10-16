#include "sensorHubConfig.h"

namespace sensor_hub {

void applyDefaultSensorHubConfig(HubConfig &config) {
    config.mqtt.enabled = false;
    config.mqtt.broker_host[0] = '\0';
    config.mqtt.broker_port = 1883;
    config.mqtt.username[0] = '\0';
    config.mqtt.password[0] = '\0';
    snprintf(config.mqtt.topic, sizeof(config.mqtt.topic), "sensorhub/telemetry");
    config.mqtt.use_tls = false;

    config.discovery.enabled = true;
    config.discovery.max_tracked_devices = 8;
    config.discovery.stale_after_ms = 300000;
}

void loadSensorHubConfig(Preferences &prefs, HubConfig &config) {
    applyDefaultSensorHubConfig(config);

    config.mqtt.enabled = prefs.getBool("sensorhub_mqtt_enabled", config.mqtt.enabled);

    String brokerHost = prefs.getString("sensorhub_mqtt_broker_host");
    if (!brokerHost.isEmpty()) {
        strlcpy(config.mqtt.broker_host, brokerHost.c_str(), sizeof(config.mqtt.broker_host));
    }

    config.mqtt.broker_port = prefs.getUShort("sensorhub_mqtt_broker_port", config.mqtt.broker_port);

    String username = prefs.getString("sensorhub_mqtt_username");
    if (!username.isEmpty()) {
        strlcpy(config.mqtt.username, username.c_str(), sizeof(config.mqtt.username));
    }

    String password = prefs.getString("sensorhub_mqtt_password");
    if (!password.isEmpty()) {
        strlcpy(config.mqtt.password, password.c_str(), sizeof(config.mqtt.password));
    }

    String topic = prefs.getString("sensorhub_mqtt_topic");
    if (!topic.isEmpty()) {
        strlcpy(config.mqtt.topic, topic.c_str(), sizeof(config.mqtt.topic));
    }

    config.mqtt.use_tls = prefs.getBool("sensorhub_mqtt_use_tls", config.mqtt.use_tls);
    config.discovery.enabled = prefs.getBool("sensorhub_discovery_enabled", config.discovery.enabled);
    config.discovery.max_tracked_devices = prefs.getUChar("sensorhub_max_tracked_devices", config.discovery.max_tracked_devices);
    config.discovery.stale_after_ms = prefs.getULong("sensorhub_stale_after_ms", config.discovery.stale_after_ms);
}

void saveSensorHubConfig(Preferences &prefs, const HubConfig &config) {
    prefs.putBool("sensorhub_mqtt_enabled", config.mqtt.enabled);
    prefs.putString("sensorhub_mqtt_broker_host", config.mqtt.broker_host);
    prefs.putUShort("sensorhub_mqtt_broker_port", config.mqtt.broker_port);
    prefs.putString("sensorhub_mqtt_username", config.mqtt.username);
    prefs.putString("sensorhub_mqtt_password", config.mqtt.password);
    prefs.putString("sensorhub_mqtt_topic", config.mqtt.topic);
    prefs.putBool("sensorhub_mqtt_use_tls", config.mqtt.use_tls);
    prefs.putBool("sensorhub_discovery_enabled", config.discovery.enabled);
    prefs.putUChar("sensorhub_max_tracked_devices", config.discovery.max_tracked_devices);
    prefs.putULong("sensorhub_stale_after_ms", config.discovery.stale_after_ms);
}

void clearSensorHubConfig(Preferences &prefs) {
    prefs.remove("sensorhub_mqtt_enabled");
    prefs.remove("sensorhub_mqtt_broker_host");
    prefs.remove("sensorhub_mqtt_broker_port");
    prefs.remove("sensorhub_mqtt_username");
    prefs.remove("sensorhub_mqtt_password");
    prefs.remove("sensorhub_mqtt_topic");
    prefs.remove("sensorhub_mqtt_use_tls");
    prefs.remove("sensorhub_discovery_enabled");
    prefs.remove("sensorhub_max_tracked_devices");
    prefs.remove("sensorhub_stale_after_ms");
}

}  // namespace sensor_hub
