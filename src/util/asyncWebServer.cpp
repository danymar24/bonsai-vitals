#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "readMoistureSensor.h"
#include "asyncWebServer.h"
#include <ArduinoJson.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        int sensorReadings = readMoistureSensor();
        notifyClients("moisture", "percentage", String(sensorReadings));
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