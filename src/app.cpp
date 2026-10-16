/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <lvgl.h>
#include "lvgl_port_v8.h"
#include "WiFi.h"
#include <ESP_IOExpander.h>


#include "./ui.h"

#include "./util/preferences.h"
#include "./util/sensorHubConfig.h"
#include "./util/sensorHubManager.h"
#include "./util/sensorHubMqtt.h"
#include <SPIFFS.h>
#include "./util/asyncWebServer.h"
#include "expander.h"

sensor_hub::SensorHubManager sensorHubManager;
sensor_hub::SensorHubMqtt sensorHubMqtt;

void setup()
{
    pinMode(MOISTURE_SENSOR_PIN, INPUT);
    prefs.begin("bonsai-vitals", false);
    String title = "Bonsai Vitals";
    Serial2.begin(115200, SERIAL_8N1, 44, 43);
    Serial2.println(title + " start");

    sensor_hub::HubConfig hubConfig;
    sensor_hub::applyDefaultSensorHubConfig(hubConfig);
    sensor_hub::loadSensorHubConfig(prefs, hubConfig);
    sensorHubMqtt.configure(hubConfig);

    String network_ssid = prefs.getString("ssid");
    String password_value = prefs.getString("password");
    prefs.end();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (!sensorHubManager.begin()) {
        Serial2.println("Sensor hub discovery failed to initialize");
    }

    if (network_ssid == "" && password_value == "")
    {
        Serial2.print("No wifi credentials");
    }
    else
    {
        WiFi.begin(network_ssid.c_str(), password_value.c_str());
    }


    if (!SPIFFS.begin(true))
    {
        Serial2.println("An Error has occurred while mounting SPIFFS");
        return;
    }


    Serial2.println("Initialize panel device");
    ESP_Panel *panel = new ESP_Panel();
    panel->init();

    panel->begin();

    Serial2.println("Initialize LVGL");
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    Serial2.println("Create UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    ui_init();

    /* Release the mutex */
    lvgl_port_unlock();


    initWebServer();
    initWebSocket();

    ExpanderInit();

    pinMode(BACKLIGHT_CONTROL_PIN, OUTPUT);

    Serial2.println(title + " end");
}

void loop()
{
    static uint8_t reportedWifiChannel = 0;
    static uint8_t announcedChannel = 0;
    static uint32_t lastChannelAnnouncementMs = 0;
    static bool wasWifiConnected = false;
    const uint32_t now = millis();
    const bool wifiConnected = WiFi.status() == WL_CONNECTED;
    uint8_t channel = WiFi.channel();
    if (channel < 1 || channel > 13) {
        channel = 1;
    }

    if (wifiConnected && channel != reportedWifiChannel) {
            Serial2.printf("Hub Wi-Fi / ESP-NOW channel: %u\n", channel);
            reportedWifiChannel = channel;
    }

    if ((wifiConnected && !wasWifiConnected) || channel != announcedChannel || now - lastChannelAnnouncementMs >= 250) {
        if (sensorHubManager.broadcastChannelAnnouncement(channel)) {
            announcedChannel = channel;
            lastChannelAnnouncementMs = now;
        }
    }
    wasWifiConnected = wifiConnected;

    sensorHubMqtt.loop();
    delay(2);
}
