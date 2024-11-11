#ifndef WiFiModule_h
#define WiFiModule_h

#include <WiFi101.h>

// Declare functions to initialize and manage WiFi
void initializeWiFi();
void printWiFiStatus(int status);
void sendSensorData(const char* server, int port, const String& data);

#endif