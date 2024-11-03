#include "../include/status.h"


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


void sendStatus(const char *message)
{
    // Implement WIFI or other communication methods here
}