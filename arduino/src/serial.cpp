#include "../include/serial.h"

/**
 * This function is now redundant and can be removed.
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
        else if (c == 'h') // Use 'h' to navigate to home screen
        {
            currentScreen = HOME_SCREEN;
            SerialMonitorInterface.println("Navigated to Home Screen.");
        }
        else if (c == 'a') // Use 'a' to navigate to activity screen
        {
            currentScreen = FALL_AND_TEMP_SCREEN;
            SerialMonitorInterface.println("Navigated to Activity Screen.");
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
