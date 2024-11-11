#include "../include/WiFiModule.h"
#include <Wifi101.h>
#include "../include/serial.h"
#include "../include/WiFiConfig.h"



void initializeWiFi() {
    WiFi.setPins(8, 2, A3, -1); // Pins for TinyDuino WiFi
    SerialMonitorInterface.print("Connecting to WiFi: ");
    SerialMonitorInterface.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        SerialMonitorInterface.print(".");
        delay(500);
    }

    SerialMonitorInterface.println("Connected to WiFi");
    printWiFiStatus(WiFi.status());
}

void printWiFiStatus(int status) {
    switch (status) {
        case WL_NO_SHIELD: SerialMonitorInterface.println("No Shield"); break;
        case WL_IDLE_STATUS: SerialMonitorInterface.println("Idle"); break;
        case WL_NO_SSID_AVAIL: SerialMonitorInterface.println("No SSID Available"); break;
        case WL_SCAN_COMPLETED: SerialMonitorInterface.println("Scan Completed"); break;
        case WL_CONNECTED: SerialMonitorInterface.println("Connected"); break;
        case WL_CONNECT_FAILED: SerialMonitorInterface.println("Connection Failed"); break;
        case WL_CONNECTION_LOST: SerialMonitorInterface.println("Connection Lost"); break;
        case WL_DISCONNECTED: SerialMonitorInterface.println("Disconnected"); break;
        default: SerialMonitorInterface.println("Unknown Status"); break;
    }
}

WiFiClient client; // Create a WiFi client instance

void sendSensorData(const char* server, int port, const String& data) {
    if (WiFi.status() != WL_CONNECTED) {
        SerialMonitorInterface.println("WiFi not connected. Attempting reconnection...");
        initializeWiFi(); // Reinitialize WiFi if not connected
    }

    SerialMonitorInterface.print("Connecting to server: ");
    SerialMonitorInterface.println(server);

    // Attempt to connect to the server
    if (client.connect(server, port)) {
        SerialMonitorInterface.println("Connected to server");

        // Construct the HTTP POST request
        client.println("POST /api/sensordata HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Content-Type: application/json"); // or "application/x-www-form-urlencoded" for URL-encoded data
        client.print("Content-Length: ");
        client.println(data.length());
        client.println(); // End of headers
        client.println(data); // Send the data payload

        // Wait for server response
        int timeout = 5000; // 5 seconds timeout
        unsigned long startTime = millis();
        while (!client.available() && (millis() - startTime < timeout)) {
            delay(100);
        }

        if (client.available()) {
            String response = client.readString();
            SerialMonitorInterface.println("Server response:");
            SerialMonitorInterface.println(response);
        } else {
            SerialMonitorInterface.println("Server response timeout.");
        }

        client.stop(); // Close the connection
    } else {
        SerialMonitorInterface.println("Connection to server failed.");
    }
}