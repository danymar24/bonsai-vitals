/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <lvgl.h>
#include "lvgl_port_v8.h"

#include "./ui.h";

void setup()
{
    pinMode(MOISTURE_SENSOR_PIN, INPUT);

    String title = "LVGL porting example";

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

    int moisture = analogRead(MOISTURE_SENSOR_PIN);
    lv_label_set_text_fmt(ui_MoistureLabel, "Moisture: %d", moisture);

    delay(1000);
}
