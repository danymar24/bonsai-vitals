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

#include "./util/preferences.h"
#include <SPIFFS.h>
#include "./util/asyncWebServer.h"

unsigned long previousMillis = 0;
const long interval = 60000; // Verify sensor every 60 seconds

void notFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS)
  {
    request->send(200);
  }
  else
  {
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
  }
}

void setup()
{
    pinMode(MOISTURE_SENSOR_PIN, INPUT);
    prefs.begin("bonsai-vitals", false);
    String title = "Bonsai Vitals";
    Serial2.begin(115200, SERIAL_8N1, 44, 43);
    Serial2.println(title + " start");

    String network_ssid = prefs.getString("ssid");
    String password_value = prefs.getString("password");
    prefs.end();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

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

    /**
     * Web server requirements
     */
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    server.serveStatic("/static/", SPIFFS, "/");
    server.onNotFound(notFound);
    server.begin();

    Serial2.println(title + " end");
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        readMoisture(NULL);
    }

    delay(2);
}
