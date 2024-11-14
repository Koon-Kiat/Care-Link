#include "../include/status.h"
#include "../include/WiFiModule.h"
#include "../include/config.h" // Include the config file for server details

/**
 * @brief Sends the fall status to the server.
 *
 * @param status The fall status string (e.g., "SAFE", "SEVERE FALL DETECTED", "MODERATE FALL DETECTED", "MINOR FALL DETECTED").
 */
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
    Serial.print("Sent to server: ");
    Serial.println(jsonData);
}