#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <WiFi101.h>

void initializeWiFi();
void printWiFiStatus(int status);
void sendSensorData(const char* server, int port, const String& data);
void processIncomingRequests();
void handleMedicationUpdate(WiFiClient& client);

#endif