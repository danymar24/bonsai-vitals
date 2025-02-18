#include <Arduino.h>
#include "../ui.h"
#include "readMoistureSensor.h"

int readMoistureSensor() 
{
    const int dry = 3000; // value for dry sensor
    const int wet = 1450; // value for wet sensor

    int moistureRead = analogRead(MOISTURE_SENSOR_PIN);
    int formattedMoisture = map(moistureRead, wet, dry, 100, 0); // Convert the soil sensor reading to percentage
    return formattedMoisture;
}