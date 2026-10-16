# Sensor Hub Migration Plan

## Goal and boundary

Repurpose this ESP32-S3 display/web device from directly sampling one soil sensor into a hub that discovers and tracks multiple ESP32 sensor broadcasters over ESP-NOW, presents their readings on the built-in display and hosted web page, and optionally forwards readings to an MQTT server. Work only in this hub repository; the sensor devices and their firmware are out of scope.

This is a planning document. All checklist items are initially unchecked, and feature implementation should not begin until Phase 0's compatibility decisions are documented.

## Phase 0 — Define contracts and preserve a baseline

- [ ] Record the supported ESP-NOW packet contract available from the sensor devices: discovery/broadcast frame, data frame, versioning, device identity, field encoding, units, optional-field representation, and validation/range rules.
- [ ] Define what “sync” means in the hub UX and persistence model (for example, selecting/allowlisting a discovered sender); do not imply a handshake or command exchange unless the existing sensor protocol supports it.
- [ ] Decide how discovery search starts/stops, how long it runs, how duplicate devices are shown, how unknown senders are handled, and the maximum number of tracked devices.
- [ ] Confirm the radio channel and Wi-Fi deployment assumptions. ESP-NOW and infrastructure Wi-Fi use the same 2.4 GHz radio/channel; define behavior for channel mismatch, Wi-Fi scans, reconnects, and access-point channel changes without changing sensor firmware.
- [ ] Define the normalized per-device data model, stale/offline threshold, and initial display behavior before the first packet arrives.
- [ ] Define WebSocket/API JSON shape and initial-state delivery so the display, web page, and MQTT output use the same canonical values and exact metric names: required `moisture_percent`; optional `temperature_c`, `humidity_percent`, `aqi`, `tvoc_ppb`, and `eco2_ppm`.
- [ ] Define MQTT broker requirements: hostname/port, optional username/password, TLS expectations, topic format, payload format, QoS/retain policy, client identity, and behavior while disabled/offline. Keep credentials out of source control.
- [ ] Capture baseline firmware and web build results, current settings behavior, and any constraints in `instructions.md` or the project README.

**Exit criteria:** protocol, discovery/sync semantics, radio channel strategy, capacity, freshness rules, and MQTT topic/payload/security expectations are written down. No sensor-side firmware work is required or permitted by this plan.

## Phase 1 — Hub data model, ESP-NOW discovery, and tracked devices

- [ ] Introduce a bounded device registry keyed by the protocol-defined identity, with discovery/sync state, most recent validated readings, and last-seen time.
- [ ] Initialize ESP-NOW in a way compatible with the hub's existing Wi-Fi station, web server, and time synchronization lifecycle.
- [ ] Implement discovery reception and search state/status for the on-device search screen; distinguish discovered devices from devices the user has synced/tracked.
- [ ] Implement user sync/select and unsync/remove behavior, persisting only the agreed device identity/configuration.
- [ ] Validate packet type/version/size, sender identity, required moisture value, optional values, and numeric ranges before updating state; ignore or report malformed and untracked packets according to Phase 0 decisions.
- [ ] Keep receive callbacks short: copy incoming data to a queue or other safe handoff, then update application state outside the callback. Do not call LVGL or MQTT from the ESP-NOW callback.
- [ ] Track last-seen/freshness and define the user-visible status for stale/offline devices.
- [ ] Verify Wi-Fi channel and reconnect behavior on hardware without relying on sensor firmware changes.

**Exit criteria:** compatible broadcasts can be discovered and synced; validated packets update the bounded canonical per-device state; radio callbacks do not block or invoke LVGL/network publishing unsafely.

## Phase 2 — On-device screens and hosted web data

- [ ] Add a Search Devices screen following the existing LVGL visual/navigation conventions, with scan status, discovered/tracked state, and clear sync/unsync feedback.
- [ ] Replace the single local-moisture home view with a useful multi-device sensor overview/detail presentation. Show required moisture and only show optional measurements when present.
- [ ] Preserve the existing settings, Wi-Fi, time, and night-display behavior unless a conflict is documented.
- [ ] Define safe LVGL update scheduling/locking for readings arriving asynchronously; make navigation and periodic refresh robust when devices are silent.
- [ ] Replace the moisture-only WebSocket event shape with the agreed canonical device update/initial-state message. Provide enough information for clients connecting after packets have already arrived.
- [ ] Update `web/src/types.ts` and `web/src/App.tsx` to render the same tracked devices and optional fields, including empty, stale/offline, and reconnecting states.
- [ ] Keep the React development build and firmware-served production bundle paths aligned through `extra_script.py`; do not manually edit generated `data/` or `build/` files.
- [ ] Verify data parity: the same device identity, values, optional-field availability, and freshness are represented on device and web clients.

**Exit criteria:** users can search/sync devices and view the same per-device measurements on the physical display and web page; no local ADC sampling remains as the source of hub readings.

## Phase 3 — MQTT settings and optional forwarding

- [x] Add an MQTT entry to the existing settings screen and an MQTT configuration screen styled consistently with Wi-Fi/time screens.
- [x] Provide enable/disable, broker and other Phase 0-approved fields, save/validation feedback, and a clear connection/status indicator.
- [x] Persist MQTT configuration using the hub's preferences storage; reset-to-factory behavior must clear these settings consistently.
- [x] Add the MQTT client/dependency only after selecting a library compatible with the project's Arduino/ESP32 core and memory constraints.
- [x] Publish canonical sensor updates only when MQTT is enabled, using the agreed topic and payload. Define whether a publish is per metric, per device snapshot, or another Phase 0-approved form; do not invent a server schema.
- [x] Implement bounded reconnect/backoff and ensure broker failures do not block ESP-NOW reception, LVGL, or the web server.
- [ ] Confirm Wi-Fi credentials and MQTT credentials are not exposed in web/status payloads or logs; document transport and storage security limits.
- [ ] Verify disable, save/reboot, bad credentials, unavailable broker, reconnect, and successful publish paths.

### Verification note
- [x] PlatformIO firmware build and filesystem upload were verified successfully after correcting the ESP32-S3 flash/partition layout for the actual 8 MB board flash.
- [x] Firmware and filesystem upload were confirmed on the target device over COM18.

**Exit criteria:** MQTT can be configured and persisted from the device UI; when enabled, received measurements are forwarded in the agreed format; when disabled or disconnected, local hub behavior remains operational.

## Phase 4 — Integration, robustness, and documentation

- [x] Build firmware with the PlatformIO `ESP-LCD` environment and resolve regressions.
- [x] Run the React production build; verify the SPIFFS build hook copies the current web output as intended.
- [ ] Test discovery with zero, one, multiple, duplicate, unknown, malformed, and silent broadcasters; test capacity and device removal.
- [ ] Test optional metrics present/absent, reconnects, stale transitions, restart persistence, and consistency between display, WebSocket/web UI, and MQTT.
- [ ] On hardware, test ESP-NOW while Wi-Fi remains connected, during network scans/reconnects, and across the supported channel arrangement.
- [ ] On hardware or against a test broker, verify MQTT disabled/enabled, correct topics/payloads, broker outage/recovery, and that MQTT does not stall other services.
- [x] Update README features, architecture, configuration, build/upload instructions, protocol assumptions, and known limitations; remove or revise obsolete direct-sensor instructions.
- [x] Document which checks require real sensor broadcasters and hardware versus checks completed in build/test tooling.

### Verification note
- [x] Verified build: `platformio run --environment ESP-LCD` exited successfully.
- [x] Verified filesystem image generation: `platformio run --target buildfs --environment ESP-LCD` succeeded.
- [x] Verified filesystem upload: `platformio run --target uploadfs --environment ESP-LCD --upload-port COM18` succeeded.
- [x] Verified firmware upload: `platformio run --target upload --upload-port COM18` succeeded.

**Exit criteria:** all agreed product behavior is validated, limitations are documented, and the implementation has no dependency on changes to sensor-device firmware.

## Cross-cutting acceptance checklist

- [ ] Hub-only scope respected; no sensor firmware files/repositories modified.
- [ ] Multiple synced senders are independently identified and displayed.
- [ ] `moisture_percent` is required for accepted sensor data; optional metrics are never fabricated when absent.
- [ ] The device display, hosted web page, and MQTT output consume the same normalized received data.
- [ ] Wi-Fi/ESP-NOW channel coexistence and callback safety are handled and tested.
- [ ] MQTT is opt-in, resilient to outages, and does not prevent local hub operation.
- [ ] Existing unrelated hub configuration/features remain functional or have documented replacements.
- [ ] Firmware and web builds pass, with hardware-only validation recorded separately.
