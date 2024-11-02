// BLE.cpp
#include "../include/ble.h"

void sendToBluetooth(const char *data)
{
    if (ble_connection_state)
    {
        // Set the ignore variable appropriately (use 0 as a placeholder)
        uint8_t ignore = 0;

        // Ensure the data fits in the buffer
        uint8_t sendLength = strlen(data);
        uint8_t sendBuffer[MAX_RECEIVED_MESSAGE_SIZE];

        // Check if the message is too long
        if (sendLength > sizeof(sendBuffer))
        {
            SerialMonitorInterface.println("Error: Message too long!");
            return;
        }

        strncpy((char *)sendBuffer, data, sizeof(sendBuffer));

        // Log the data being sent
        SerialMonitorInterface.print("Sending data over BLE: ");
        SerialMonitorInterface.println(data);

        // Send the data
        uint8_t result = lib_aci_send_data(ignore, sendBuffer, sendLength);
        if (result != 0)
        { // Assuming 0 is the success code
            SerialMonitorInterface.print("Failed to send data over BLE, error code: ");
            SerialMonitorInterface.println(result);
        }

        delay(1000);
    }
    else
    {
        SerialMonitorInterface.println("BLE connection is not established.");
    }
}