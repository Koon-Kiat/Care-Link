#include "../include/status.h"
#include "../include/wifi_module.h"
#include "../include/wifi_config.h"
#include "../include/config.h"
#include <ArduinoJson.h>

/**
 * @brief Sends all sensor data to the server in JSON format.
 * 
 * @param fallStatus The status of the fall sensor.
 * @param activityStatus The status of the activity sensor.
 * @param temperature The temperature reading.
 * @param timestamp The timestamp of the sensor data.
 * @param medStatus The status of the medication sensor.
 * @param panicStatus The status of the panic sensor.
 * 
 * This function creates a JSON object containing the provided sensor data,
 * serializes it to a string, and sends it to the server. It also logs the
 * JSON payload to the serial monitor for debugging purposes.
 */
void sendAllSensorData(const char *fallStatus, const char *activityStatus, double temperature, const char *timestamp, const char *medStatus, const char *panicStatus) {
   
    // Create a JSON object for the data
    StaticJsonDocument<256> jsonDoc;

    // Add the sensor data to the JSON object
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

/**
 * @brief Sends the fall status message to the server.
 * 
 * @param status The fall status message.
 * 
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
 * @brief Sends the temperature status message to the server.
 * 
 * @param temperature The temperature reading.
 * 
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
 * @brief Sends the medication status message to the server.
 * 
 * @param medStatus The medication status string.
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
 * @brief Sends the panic status message to the server.
 * 
 * @param panicStatus The panic status message.
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
 * @param message The status message.
 * 
 * This function constructs a JSON payload with the provided status message
 * and sends it to the server. It also logs the JSON payload to the serial
 * monitor for debugging purposes.
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