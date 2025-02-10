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

#include "./ui.h"
#include "./events/events.h"


#include "./util/preferences.h"

void setup()
{
    pinMode(MOISTURE_SENSOR_PIN, INPUT);
    
    prefs.begin("bonsai-vitals", false);

    String network_ssid = prefs.getString("ssid");
	String password_value = prefs.getString("password");
    prefs.end();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if(network_ssid == "" && password_value == "") {
        Serial.print("No wifi credentials");
    } else {
        WiFi.begin(network_ssid.c_str(), password_value.c_str());
    }

    String title = "Bonsai Vitals";

    Serial.begin(115200);
    Serial.println(title + " start");

    Serial.println("Initialize panel device");
    ESP_Panel *panel = new ESP_Panel();
    panel->init();

    panel->begin();

    Serial.println("Initialize LVGL");
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    Serial.println("Create UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    ui_init();

    /* Release the mutex */
    lvgl_port_unlock();

    Serial.println(title + " end");
}

void loop()
{
    Serial.println("IDLE loop");

    readMoisture();

    getWifiStatus();

    delay(1000);
}
