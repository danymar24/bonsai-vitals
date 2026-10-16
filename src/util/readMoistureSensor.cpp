#include <Arduino.h>
#include "../ui.h"
#include "readMoistureSensor.h"

const int numReadings = 10;
int readings[numReadings];
int readingNum = 0;
int sum = 0;
float average = 0;

int readMoistureSensor() 
{
    const int dry = 2170; // value for dry sensor
    const int wet = 1600; //1510 // value for wet sensor
    int sensorRead = analogRead(MOISTURE_SENSOR_PIN);
    Serial2.println(sensorRead);
    readings[readingNum] = sensorRead;

    sum += readings[readingNum];

    readingNum = (readingNum + 1) % numReadings;

    if(readingNum == 0) {
        average = sum / numReadings;
        sum = 0;
    }

    if(average == 0){
        average = readings[0];
    }

    int formattedMoisture = map(average, wet, dry, 100, 0); // Convert the soil sensor reading to percentage
    return formattedMoisture;
}