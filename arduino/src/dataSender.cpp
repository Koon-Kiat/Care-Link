#include "../include/dataSender.h"
#include "../include/WiFiModule.h" // For sendSensorData()
#include "../include/WiFiConfig.h" // For SERVER_ADDRESS and SERVER_PORT
#include "../include/jsonHandler.h" // For createJsonPayload()

void sendDataToServer(const String &fallStatus, const String &activityStatus, float temperature) {
    // Create a JSON payload using the JSON handler module
    String sensorData = createJsonPayload(fallStatus, activityStatus, temperature);

    // Send the JSON data to the server
    sendSensorData(SERVER_ADDRESS, SERVER_PORT, sensorData);
}
