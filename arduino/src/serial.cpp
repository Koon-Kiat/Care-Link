// src/Serial.cpp
#include "../include/serial.h"

/**
 * @brief Handles serial input from the Serial Monitor.
 *        Responds to specific commands like 's' and 'q'.
 */
void handleSerialInput()
{
    if (SerialMonitorInterface.available() > 0)
    {
        char c = SerialMonitorInterface.read();
        if (c == 's') // Use 's' for sending status updates
        {
            sendFallStatus("STATUS REQUESTED\n");
        }
        else if (c == 'q') // Use 'q' for quitting
        {
            SerialMonitorInterface.println("Quit requested!");
            while (1)
                ; // Hang the system for quitting
        }
    }
}

/**
 * @brief Displays sensor values on the Serial Monitor.
 */
void showSerial()
{
    // Print sensor values to Serial Monitor
    SerialMonitorInterface.print("X: ");
    SerialMonitorInterface.print(x);
    SerialMonitorInterface.print(", Y: ");
    SerialMonitorInterface.print(y);
    SerialMonitorInterface.print(", Z: ");
    SerialMonitorInterface.print(z);
    SerialMonitorInterface.print(", Temperature: ");
    SerialMonitorInterface.print(temp);
    SerialMonitorInterface.println(" C");
}