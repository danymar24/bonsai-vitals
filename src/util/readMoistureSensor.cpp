#include <Arduino.h>
#include "../ui.h"
#include "readMoistureSensor.h"

const int numReadings = 30;
int readings[numReadings];
int readingNum = 0;
int sum = 0;
float average;

int readMoistureSensor() 
{
    const int dry = 1900; // value for dry sensor
    const int wet = 1510; // value for wet sensor

    readings[readingNum] = analogRead(MOISTURE_SENSOR_PIN);

    sum += readings[readingNum];

    readingNum = (readingNum + 1) % numReadings;

    if(readingNum == 0) {
        average = sum / numReadings;
        sum = 0;
    }

    int formattedMoisture = map(average, wet, dry, 100, 0); // Convert the soil sensor reading to percentage
    return formattedMoisture;
}