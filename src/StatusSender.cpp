#include "StatusSender.h"
#include "BLE.h" // Assuming sendToBluetooth is defined here

#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

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
    sendToBluetooth(message.c_str());
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

    sendToBluetooth(message.c_str());
}
