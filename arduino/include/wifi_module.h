#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <WiFi101.h>

// Declare functions to initialize and manage WiFi
void initializeWiFi();
void printWiFiStatus(int status);
void sendSensorData(const char* server, int port, const String& data);

#endif