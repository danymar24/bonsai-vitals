# Bonsai Vitals Sensor Hub

This repository has been repurposed from a single local soil-sensor device into a hub that listens for ESP32 sensor broadcasts over ESP-NOW, tracks multiple devices, shows their readings on the built-in display and hosted web UI, and can optionally forward the same normalized state to MQTT.

* Display: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3
* LVGL UI: https://lvgl.io/
* Web client: React + TypeScript
* Radio: ESP-NOW with Wi-Fi station mode

## Features

* Discovers and tracks multiple ESP32 sensor devices over ESP-NOW
* Maintains a normalized device registry keyed by sender identity
* Shows the latest received values on the LVGL display
* Serves a hosted web client with the same hub-state payload
* Supports optional MQTT forwarding for canonical sensor snapshots
* Persists MQTT and discovery settings in Preferences/NVS
* Keeps the UI and WebSocket payload aligned around the same sensor names

## Core contract

The hub uses a normalized device-state model with these exact metric names when present:

* `moisture_percent` (required for accepted data)
* `temperature_c`
* `humidity_percent`
* `aqi`
* `tvoc_ppb`
* `eco2_ppm`

Optional values are omitted when unavailable instead of being fabricated as zeros.

## Development

### UI
For the UI, SquareLine Studio is used for the generated LVGL source, and the generated files remain under `src/`.

### Firmware
The firmware is built with PlatformIO using the `ESP-LCD` environment defined in `platformio.ini`.

### Web app
The web app is built using React. Typical development flow:

* `cd web`
* `npm install`
* `npm run start`

Production build status:

* `npm --prefix web run build` is currently passing in this workspace

### SPIFFS build hook
The `extra_script.py` hook builds the web app and copies the production output into the generated SPIFFS `data/` directory during the PlatformIO build flow.

## MQTT

MQTT is opt-in. When enabled, the hub publishes the same canonical JSON payload it serves over the WebSocket API to the configured broker/topic.

Important security notes:

* Broker credentials are stored in the device Preferences store, not hard-coded in source.
* Credentials are not included in the served web status payloads or MQTT topic payloads.
* HTTPS/TLS is not enabled by default in the current config; use the device settings and broker configuration according to the deployment environment.

## Build and validation status

Validated in this workspace:

* React production build passes via `npm --prefix web run build`

Hardware-only checks remain separate from build verification and must be done on the target ESP32-S3 hub with compatible sensor devices:

* ESP-NOW discovery and packet validation
* Wi-Fi and ESP-NOW coexistence while the hub remains connected to a network
* MQTT enable/disable and broker outage/recovery behavior
* End-to-end display/web parity on real hardware

## Historical note

The original project used a direct capacitive soil sensor attached to the ESP32. This repo has been migrated to a hub architecture and should not be treated as a direct-sensor project anymore.

This work remains scoped to the hub repository only; the ESP32 sensor firmware is out of scope for this project.