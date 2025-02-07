# Bonsai Vitals

A Project to monitor a bonsai vitals using a Waveshare display and ESP32.

* Display: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3
* LVGL for the UI: https://lvgl.io/

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

Based on the sample of [esp-arduino-libs/ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel)