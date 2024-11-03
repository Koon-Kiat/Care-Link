// src/SensorHandler.cpp
#include "../include/fall_and_temperature_sensor.h"
#include <math.h>
#include "../include/bma250.h"
#include "../include/display.h"
#include "../include/serial.h"

void checkFallDetectionAndTemperature()
{
    accel_sensor.read(); // Update acceleration values
    x = accel_sensor.X;
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0); // Convert raw temperature to Celsius

    // Define fall thresholds for categorization
    const int16_t FALL_MINOR_THRESHOLD_LOW = -400;
    const int16_t FALL_MODERATE_THRESHOLD_LOW = -1000;
    const int16_t FALL_SEVERE_THRESHOLD = -1500;

    const int16_t FALL_MINOR_THRESHOLD_HIGH = 400;     // Positive threshold for minor rise
    const int16_t FALL_MODERATE_THRESHOLD_HIGH = 1000; // Positive threshold for moderate rise
    const int16_t FALL_SEVERE_THRESHOLD_HIGH = 1500;   // Positive threshold for severe rise

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100)
    {           // Check every 100ms
        return; // Skip this loop if within the sampling period
    }
    lastCheckTime = currentTime; // Update the last check time

    // Define thresholds for activity detection
    const double RESTING_THRESHOLD = 248.0; // Adjusted based on observed resting magnitude
    const double WALKING_THRESHOLD = 260.0; // Adjusted based on testing
    const double RUNNING_THRESHOLD = 300.0; // Adjusted based on testing

    // Calculate magnitude
    double magnitude = sqrt(x * x + y * y + z * z);
    String activityStatus;

    if (magnitude < RESTING_THRESHOLD)
    {
        activityStatus = "RESTING"; // Indicates rest
    }
    else if (magnitude >= RESTING_THRESHOLD && magnitude < WALKING_THRESHOLD)
    {
        activityStatus = "WALKING";
    }
    else if (magnitude >= WALKING_THRESHOLD)
    {
        activityStatus = "RUNNING";
    }

    // Check thresholds for fall detection
    if (z <= FALL_SEVERE_THRESHOLD || z >= FALL_SEVERE_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Severe fall detected!");
        sendFallStatus("SEVERE FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else if (z <= FALL_MODERATE_THRESHOLD_LOW || z >= FALL_MODERATE_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Moderate fall detected!");
        sendFallStatus("MODERATE FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else if (z <= FALL_MINOR_THRESHOLD_LOW || z >= FALL_MINOR_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Minor fall detected!");
        sendFallStatus("MINOR FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else
    {
        sendFallStatus("SAFE\n");
    }

    // Display activity status and temperature on TinyScreen
    displayActivityStatus(activityStatus.c_str(), temp);
    sendTemperatureStatus(temp);

    if (x == -1 && y == -1 && z == -1)
    {
        // Print error message to Serial Monitor
        SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    else
    {
        showSerial();
    }

    delay(1000); // Delay for stability
}