#include "../include/wifi_module.h"
#include <Wifi101.h>
#include "../include/serial.h"
#include "../include/wifi_config.h"

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

WiFiClient client;

void sendSensorData(const char* server, int port, const String& data) {
    SerialMonitorInterface.println("Starting data transmission...");

    if (WiFi.status() != WL_CONNECTED) {
        SerialMonitorInterface.println("WiFi is not connected. Reconnecting...");
        initializeWiFi();
    }

    SerialMonitorInterface.print("Connecting to server: ");
    SerialMonitorInterface.println(server);

    if (client.connect(server, port)) {
        SerialMonitorInterface.println("Connected to server!");

        // Construct HTTP POST request
        client.println("POST /api/sensordata HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(data.length());
        SerialMonitorInterface.print("Payload length: ");
        SerialMonitorInterface.println(data.length());
        client.println(); 
        client.println(data);
        SerialMonitorInterface.println("Payload sent to the server.");

        // Log the full request
        SerialMonitorInterface.println("HTTP Request:");
        SerialMonitorInterface.println("POST /api/sensordata HTTP/1.1");
        SerialMonitorInterface.print("Host: ");
        SerialMonitorInterface.println(server);
        SerialMonitorInterface.println("Content-Type: application/json");
        SerialMonitorInterface.print("Content-Length: ");
        SerialMonitorInterface.println(data.length());
        SerialMonitorInterface.println(data);

        // Wait for server response
        unsigned long timeout = millis() + 5000;
        while (!client.available() && millis() < timeout) {
            delay(100);
        }

        if (client.available()) {
            String response = client.readString();
            SerialMonitorInterface.println("Server Response:");
            SerialMonitorInterface.println(response);
        } else {
            SerialMonitorInterface.println("Server response timeout.");
        }

        client.stop();
    } else {
        SerialMonitorInterface.println("Failed to connect to server.");
    }
}
