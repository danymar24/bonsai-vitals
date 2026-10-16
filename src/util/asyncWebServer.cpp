#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "readMoistureSensor.h"
#include "asyncWebServer.h"
#include "sensorHubManager.h"
#include "sensorHubMqtt.h"
#include <ArduinoJson.h>

extern sensor_hub::SensorHubManager sensorHubManager;
extern sensor_hub::SensorHubMqtt sensorHubMqtt;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        publishHubStateToClients();
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial2.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial2.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void publishHubStateToClients() {
    JsonDocument message;
    message["name"] = "sensor_hub";
    message["type"] = "state";

    JsonObject state = message["value"].to<JsonObject>();
    state["device_count"] = sensorHubManager.getTrackedDeviceCount();

    JsonArray devices = state.createNestedArray("devices");
    for (uint8_t i = 0; i < sensorHubManager.getTrackedDeviceCount(); ++i) {
        sensor_hub::DiscoveredDevice device;
        if (!sensorHubManager.getDeviceAt(i, device)) {
            continue;
        }

        JsonObject entry = devices.add<JsonObject>();
        entry["device_id"] = device.device_id;
        entry["is_discovered"] = device.is_discovered;
        entry["is_synced"] = device.is_synced;
        entry["last_seen_ms"] = device.last_seen_ms;

        if (!isnan(device.latest.moisture_percent)) {
            entry["moisture_percent"] = device.latest.moisture_percent;
        }
        if (!isnan(device.latest.temperature_c)) {
            entry["temperature_c"] = device.latest.temperature_c;
        }
        if (!isnan(device.latest.humidity_percent)) {
            entry["humidity_percent"] = device.latest.humidity_percent;
        }
        if (!isnan(device.latest.aqi)) {
            entry["aqi"] = device.latest.aqi;
        }
        if (!isnan(device.latest.tvoc_ppb)) {
            entry["tvoc_ppb"] = device.latest.tvoc_ppb;
        }
        if (!isnan(device.latest.eco2_ppm)) {
            entry["eco2_ppm"] = device.latest.eco2_ppm;
        }
    }

    sensor_hub::SensorReading reading;
    if (sensorHubManager.getLatestSyncedReading(reading)) {
        if (!isnan(reading.moisture_percent)) {
            state["moisture_percent"] = reading.moisture_percent;
        }
        if (!isnan(reading.temperature_c)) {
            state["temperature_c"] = reading.temperature_c;
        }
        if (!isnan(reading.humidity_percent)) {
            state["humidity_percent"] = reading.humidity_percent;
        }
        if (!isnan(reading.aqi)) {
            state["aqi"] = reading.aqi;
        }
        if (!isnan(reading.tvoc_ppb)) {
            state["tvoc_ppb"] = reading.tvoc_ppb;
        }
        if (!isnan(reading.eco2_ppm)) {
            state["eco2_ppm"] = reading.eco2_ppm;
        }
    }

    char json_string[1024];
    size_t len = serializeJson(message, json_string, sizeof(json_string));
    if (len > 0 && ws.count() > 0) {
        ws.textAll(json_string);
    }

    if (len > 0 && sensorHubMqtt.isEnabled()) {
        sensorHubMqtt.publishJsonMessage(json_string);
    }
}

void initWebSocket()
{
    Serial2.print("Websocket initiating...");
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void notFound(AsyncWebServerRequest *request)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    else
    {
        request->send(404, "application/json", "{\"message\":\"Not found\"}");
    }
}

void notifyClients(String name, String type, String value) {
    if(sizeof(ws.getClients()) > 0)
    {
        JsonDocument message;
        message["name"] = name;
        message["type"] = type;
        message["value"] = value;
        char json_string[256];
        serializeJson(message, json_string);
        ws.textAll(json_string);
    }
}

void initWebServer()
{
    Serial2.print("Webserver initiating...");

    /**
     * Web server requirements
     */
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    server.serveStatic("/static/", SPIFFS, "/");
    server.onNotFound(notFound);
    server.begin();
}