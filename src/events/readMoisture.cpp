#include <WiFi.h>
#include "ui.h"

const int dry = 4095; // value for dry sensor
const int wet = 2300; // value for wet sensor

static void readMoisture()
{
    int moistureRead = analogRead(MOISTURE_SENSOR_PIN);
    int formattedMoisture = map(moistureRead, wet, dry, 100, 0); // Convert the soil sensor reading to percentage

    lv_arc_set_value(ui_MoistureGauge, formattedMoisture);
    lv_label_set_text_fmt(ui_MoisturePercentageLabel, "%d%%", formattedMoisture);
    lv_label_set_text_fmt(ui_MoistureLabel, "Moisture", formattedMoisture);
}