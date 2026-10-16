# Project Instructions

## Project direction

This repository currently contains firmware for a Waveshare ESP32-S3 display hub and a React web page. The existing firmware reads a directly connected capacitive soil-moisture sensor. The intended direction is to repurpose this device as a **sensor hub** that receives measurements from multiple ESP32 sensor devices over ESP-NOW, displays them on the device and its hosted web page, and can optionally forward them to an MQTT server.

Read [SENSOR_HUB_PLAN.md](SENSOR_HUB_PLAN.md) before implementing this change. The plan is intentionally staged; complete the protocol and behavior decisions in Phase 0 before writing feature code.

## Scope boundaries

- Work only on this hub repository: its firmware, on-device UI, hosted web UI, configuration, and documentation.
- Do not modify, build, or add requirements for the ESP32 sensor-device firmware. Treat the ESP-NOW packet contract as an external compatibility boundary.
- Replace the hub's local-sensor reading behavior with reception and presentation of sensor-device data. Preserve unrelated existing hub features unless the migration requires a documented change.
- MQTT is optional and forwards received sensor data from the hub to the configured server; it is not a requirement for sensor devices.

## Required product behavior

- Support discovery/search for multiple ESP32 devices that broadcast over ESP-NOW, and let the user sync/select devices for the hub to track. Do not assume discovery, pairing, identity, or packet fields that are not defined by the sensor-side contract.
- Present measurements per tracked device: `moisture_percent` is required; `temperature_c`, `humidity_percent`, `aqi`, `tvoc_ppb`, and `eco2_ppm` are optional.
- Represent unavailable optional values as unavailable (for example, omitted or `null` according to the agreed wire/API contract), not as fabricated zero readings.
- Show the same received device data on the built-in LVGL display and the web page served by this hub.
- Add an MQTT configuration entry from device settings and a matching configuration screen consistent with existing settings screens. Persist settings and publish received sensor information only when MQTT is enabled.

## Repository map and implementation constraints

- Firmware entry point and Wi-Fi/SPIFFS/UI/web startup: `src/app.cpp`.
- Current moisture-only sampling and UI updates: `src/util/readMoistureSensor.cpp`, `src/ui_events.cpp`, and `src/screens/ui_MoistureScreen.c`.
- LVGL UI declarations, screen navigation, and event hooks: `src/ui.h`, `src/ui.c`, `src/ui_events.h`, and `src/ui_events.cpp`.
- Settings, Wi-Fi, and time screens: `src/screens/ui_SettingsScreen.c`, `src/screens/ui_WiFiScreen.c`, and `src/screens/ui_TimeScreen.c`.
- NVS preferences wrapper: `src/util/preferences.h`.
- HTTP/WebSocket server and current moisture message: `src/util/asyncWebServer.cpp` and `src/util/asyncWebServer.h`.
- React client and current socket payload type: `web/src/App.tsx` and `web/src/types.ts`.
- The React production build is copied into `data/` for SPIFFS by `extra_script.py`; do not treat generated build output as the source of truth. Make web changes under `web/src/`.
- The project targets an ESP32-S3 custom board and uses Arduino via PlatformIO, LVGL 8.3, ESPAsyncWebServer, ArduinoJson, and React/TypeScript. PlatformIO is the firmware build path. The `src/CMakeLists.txt` and `src/filelist.txt` also explicitly list UI sources; keep these lists aligned when adding/removing UI source files.

## Engineering rules for implementation

- Keep one normalized, device-keyed sensor state model as the source of truth for the LVGL UI, WebSocket/API output, and MQTT publishing. Include source identity and freshness/last-seen information so stale or offline data is distinguishable from current data.
- Keep ESP-NOW receive callbacks short and non-blocking. Do not call LVGL or perform MQTT/network publishing directly from a radio callback. Copy/validate incoming data and hand it to safe application processing; update LVGL only from a context protected by the project's LVGL locking rules.
- Explicitly handle Wi-Fi/ESP-NOW coexistence: station Wi-Fi and ESP-NOW share the radio/channel, and Wi-Fi network scans or reconnects can disrupt reception. Define and test channel behavior against the fixed sensor contract; do not silently change the sensor devices.
- Validate packet length, version/type, numeric ranges, and device identity before accepting a reading. Define duplicate, unknown, stale, and malformed packet behavior in the protocol phase.
- Use bounded storage for tracked devices and avoid unbounded message/history growth. Confirm device capacity and persistence behavior before implementation.
- Keep MQTT operations non-blocking with reconnect/backoff behavior; do not let a broker outage stop ESP-NOW reception, the display, or the web server. Store credentials in the existing preferences mechanism as appropriate, never hard-code secrets, and document any security limitations of the chosen storage/transport.
- Treat optional metric absence explicitly across firmware, WebSocket/API JSON, and TypeScript types. Keep field names consistent with the agreed contract, including the exact snake_case measurement names above.
- Existing LVGL source files are SquareLine-generated. Prefer updating/regenerating the SquareLine UI source consistently; if generated files are edited manually, keep declarations, event registration, screen initialization, and source lists synchronized and document the regeneration implications.
- Keep unrelated generated artifacts (`build/`, `web/build/`, and `data/`) out of hand-authored implementation changes unless the documented build/upload process explicitly requires regeneration.
- Do not claim completion without checking the firmware build and React production build, and exercising protocol, UI/web data parity, and MQTT disabled/enabled behavior as laid out in the plan.

## Build and validation references

- Firmware build: run the existing PlatformIO `ESP-LCD` environment (`platformio.ini`).
- Web development and production build: use the scripts in `web/package.json` (`npm start` and `npm run build` from `web/`).
- The PlatformIO SPIFFS pre-build hook builds the React app and copies its production output into `data/`.
- Record hardware-only checks separately from compile/build checks; ESP-NOW reception and Wi-Fi/MQTT coexistence require validation on the target hub and compatible sensor broadcasters.
