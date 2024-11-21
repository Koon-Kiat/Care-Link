#include "../include/status.h"
#include "../include/wifi_module.h"
#include "../include/wifi_config.h"
#include "../include/config.h"
#include <ArduinoJson.h>

/**
 * @brief Sends the fall status to the server.
 *
 * @param status The fall status string (e.g., "SAFE", "SEVERE FALL DETECTED", "MODERATE FALL DETECTED", "MINOR FALL DETECTED").
 */

void sendAllSensorData(const char *fallStatus, const char *activityStatus, double temperature, const char *timestamp, const char *medStatus, const char *panicStatus) {
    // Create a JSON object for the data
    StaticJsonDocument<256> jsonDoc;

    jsonDoc["fall_status"] = fallStatus;
    jsonDoc["activity_status"] = activityStatus;
    jsonDoc["temperature"] = temperature;
    jsonDoc["timestamp"] = timestamp;
    jsonDoc["med_status"] = medStatus;
    jsonDoc["panic_status"] = panicStatus;



    // Serialize the JSON object to a string
    String jsonData;
    serializeJson(jsonDoc, jsonData);

    // Debugging: Log the JSON payload
    SerialMonitorInterface.println("Preparing to send the following JSON payload:");
    SerialMonitorInterface.println(jsonData);

    // Send the JSON payload to the server
    sendSensorData(SERVER_ADDRESS, SERVER_PORT, jsonData);

    // Debug output
    SerialMonitorInterface.print("Sent to server: ");
    SerialMonitorInterface.println(jsonData);
}

void sendFallStatus(const char *status)
{
    String message;

    if (strcmp(status, "SAFE\n") == 0)
    {
        message = "FALL: S"; // SAFE
    }
    else if (strcmp(status, "SEVERE FALL DETECTED\n") == 0)
    {
        message = "FALL: SV"; // Severe Fall
    }
    else if (strcmp(status, "MODERATE FALL DETECTED\n") == 0)
    {
        message = "FALL: M"; // Moderate Fall
    }
    else if (strcmp(status, "MINOR FALL DETECTED\n") == 0)
    {
        message = "FALL: MI"; // Minor Fall
    }

    SerialMonitorInterface.print("Preparing to send: ");
    SerialMonitorInterface.println(message.c_str());
    sendStatus(message.c_str());
}

/**
 * @brief Sends the temperature status to the server.
 *
 * @param temperature The current temperature in Celsius.
 */
void sendTemperatureStatus(double temperature)
{
    String message = "TEMP: ";

    // Categorize the temperature into short codes
    if (temperature < 28)
    {
        message += "C"; // COOL
    }
    else if (temperature >= 28 && temperature <= 33)
    {
        message += "N"; // NORMAL
    }
    else
    {
        message += "H"; // HOT
    }

    SerialMonitorInterface.print("Preparing to send: ");
    SerialMonitorInterface.println(message.c_str());
    sendStatus(message.c_str());
}

/**
 * @brief Sends the medication status to the server.
 *
 * @param medStatus The medication status string ("MED_CONFIRM" or "MED_CANCEL").
 */
void sendMedicationStatus(const char *medStatus)
{
    String message;

    if (strcmp(medStatus, "MED_CONFIRM") == 0)
    {
        message = "MED: C"; // Confirmed
    }
    else if (strcmp(medStatus, "MED_CANCEL") == 0)
    {
        message = "MED: CN"; // Canceled
    }

    SerialMonitorInterface.print("Preparing to send: ");
    SerialMonitorInterface.println(message.c_str());
    sendStatus(message.c_str());
}

/**
 * @brief Sends the panic button status message to the server.
 *
 * @param panicStatus The panic button status string.
 */
void sendPanicStatus(const char *panicStatus)
{
    SerialMonitorInterface.print("Preparing to send: ");
    SerialMonitorInterface.println(panicStatus);
    sendStatus(panicStatus);
}

/**
 * @brief Sends the status message to the server.
 *
 * @param message The status message to be sent.
 */
void sendStatus(const char *message) {
    // Construct the JSON payload
    String jsonData = "{";
    jsonData += "\"status\": \"" + String(message) + "\"";
    jsonData += "}";

    // Send the JSON payload to the server
    sendSensorData(SERVER_ADDRESS, SERVER_PORT, jsonData);

    // Debug output
    SerialMonitorInterface.print("Sent to server: ");
    SerialMonitorInterface.println(jsonData);
}