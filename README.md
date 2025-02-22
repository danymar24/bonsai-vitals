# Bonsai Vitals

A Project to monitor a bonsai vitals using a Waveshare display and ESP32.

* Display: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3
* LVGL for the UI: https://lvgl.io/

## Features

* Gets moisture from a capacitive soil sensor
* Shows the moisture in the built in screen
* Turns off screen during night if activated
* Sync time with internet
* Shows moisture in a webpage using the device ip via wifi

## Dependencies

* [Soil sensor](https://www.amazon.com/gp/product/B093V62VXD/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* [Water Pump](https://www.amazon.com/gp/product/B093V62VXD/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

## Development

### UI
For the UI, I am using [SquareLine Studio](https://squareline.io/)

### Arduino
For the logic and c/cpp code, I am using VSCode and platformio.

## Sensors

### Soil Moisture sensor

The Soil sensor is connected to the pin 6, this can be changed in the `lv_config.h` file.

```h
#define MOISTURE_SENSOR_PIN 6
```

## Web

A web version is available to view the vitals and configuration.

### Development

The web app is built using React, to start developing follow this steps:
* `cd web`
* `npm install`
* `npm run start`
* Go to the file `App.tsx` and modify it

### Push files

To push the files, go to the Platformio tab in VSCode > ESP-LCD > Platform > Upload Filesystem Image

This steps will compile the React app and push it through SPIFFS.

Based on the sample of [esp-arduino-libs/ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel)