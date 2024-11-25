#include "../include/wifi_module.h"
#include <Wifi101.h>
#include "../include/serial.h"
#include "../include/wifi_config.h"
#include <ArduinoJson.h>
#include "../include/medication.h"

WiFiServer server(80);

void initializeWiFi()
{
    WiFi.setPins(8, 2, A3, -1);
    SerialMonitorInterface.print("Connecting to WiFi: ");
    SerialMonitorInterface.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        SerialMonitorInterface.print(".");
        delay(500);
    }

    SerialMonitorInterface.println("Connected to WiFi");
    printWiFiStatus(WiFi.status());
}

void printWiFiStatus(int status)
{
    switch (status)
    {
    case WL_NO_SHIELD:
        SerialMonitorInterface.println("No Shield");
        break;
    case WL_IDLE_STATUS:
        SerialMonitorInterface.println("Idle");
        break;
    case WL_NO_SSID_AVAIL:
        SerialMonitorInterface.println("No SSID Available");
        break;
    case WL_SCAN_COMPLETED:
        SerialMonitorInterface.println("Scan Completed");
        break;
    case WL_CONNECTED:
        SerialMonitorInterface.println("Connected");
        break;
    case WL_CONNECT_FAILED:
        SerialMonitorInterface.println("Connection Failed");
        break;
    case WL_CONNECTION_LOST:
        SerialMonitorInterface.println("Connection Lost");
        break;
    case WL_DISCONNECTED:
        SerialMonitorInterface.println("Disconnected");
        break;
    default:
        SerialMonitorInterface.println("Unknown Status");
        break;
    }
}

WiFiClient client;

void sendSensorData(const char *server, int port, const String &data)
{
    SerialMonitorInterface.println("Starting data transmission...");

    if (WiFi.status() != WL_CONNECTED)
    {
        SerialMonitorInterface.println("WiFi is not connected. Reconnecting...");
        initializeWiFi();
    }

    SerialMonitorInterface.print("Connecting to server: ");
    SerialMonitorInterface.println(server);

    if (client.connect(server, port))
    {
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
        while (!client.available() && millis() < timeout)
        {
            delay(100);
        }

        if (client.available())
        {
            String response = client.readString();
            SerialMonitorInterface.println("Server Response:");
            SerialMonitorInterface.println(response);
        }
        else
        {
            SerialMonitorInterface.println("Server response timeout.");
        }

        client.stop();
    }
    else
    {
        SerialMonitorInterface.println("Failed to connect to server.");
    }
}

void processIncomingRequests()
{
    // Check for new client
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("New client connected.");

        // Read the HTTP request line
        String request = client.readStringUntil('\r');
        Serial.println("Received request:");
        Serial.println(request);

        // Check if the request is to update the medication
        if (request.indexOf("POST /update_medication") != -1)
        {
            handleMedicationUpdate(client);
        }

        client.stop();
        Serial.println("Client disconnected.");
    }
}

void handleMedicationUpdate(WiFiClient &client)
{
    StaticJsonDocument<512> jsonDoc;
    String requestBody = "";

    // Skip HTTP headers
    while (client.available())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
            break;
        }
    }

    // Read the JSON payload
    while (client.available())
    {
        requestBody += client.readString();
    }

    // Log the raw payload for debugging
    Serial.println("Received JSON payload:");
    Serial.println(requestBody);

    // Parse the JSON payload
    DeserializationError error = deserializeJson(jsonDoc, requestBody);
    if (error)
    {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Clear the current medication schedule
    medicationSchedule.clear();

    // Update the medication schedule
    JsonArray schedule = jsonDoc["schedule"];
    for (JsonObject med : schedule)
    {
        MedicationSchedule newMed;
        newMed.type = med["name"].as<String>();
        newMed.time = med["time"].as<String>();
        medicationSchedule.push_back(newMed);
    }

    // Print the updated medication schedule
    Serial.println("Updated Medication Schedule:");
    for (const auto &med : medicationSchedule)
    {
        Serial.print("Medication: ");
        Serial.print(med.type);
        Serial.print(", Time: ");
        Serial.println(med.time);
    }
}
