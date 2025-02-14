#ifndef ASYNC_WEB_SERVER_H
#define ASYNC_WEB_SERVER_H

void initWebSocket();
void initWebServer();
void notifyClients(String name, String type, String value);


#endif