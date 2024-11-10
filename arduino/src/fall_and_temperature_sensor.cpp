#include "../include/fall_and_temperature_sensor.h"
#include "../include/bma250.h"
#include "../include/display.h"
#include "../include/serial.h"
#include <math.h>

/**
 * @brief Checks for fall detection and temperature status.
 *
 * This function reads the acceleration values from the BMA250 sensor and calculates the magnitude.
 * It then compares the magnitude to predefined thresholds to determine if a fall has occurred.
 * If a fall is detected, the function sets the activity status to the appropriate message and initiates the fall display.
 *
 */
void checkFallDetectionAndTemperature()
{
    accel_sensor.read();
    x = accel_sensor.X;
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

    // Define fall thresholds for categorization
    const double FALL_MINOR_THRESHOLD = 400.0;
    const double FALL_MODERATE_THRESHOLD = 600.0;
    const double FALL_SEVERE_THRESHOLD = 800.0;

    // Define thresholds for activity detection
    const double RESTING_THRESHOLD = 300.0;
    const double WALKING_THRESHOLD = 450.0;
    const double RUNNING_THRESHOLD = 650.0;

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100)
    {
        return;
    }
    lastCheckTime = currentTime;

    // Calculate magnitude
    double magnitude = sqrt(x * x + y * y + z * z);

    static double prevMagnitude = magnitude;

    double deltaMagnitude = abs(magnitude - prevMagnitude);

    prevMagnitude = magnitude;

    SerialMonitorInterface.println(magnitude);
    SerialMonitorInterface.println(deltaMagnitude);

    if (magnitude < RESTING_THRESHOLD)
    {
        activityStatus = "RESTING";
    }
    else if (magnitude < WALKING_THRESHOLD)
    {
        activityStatus = "WALKING";
    }
    else
    {
        activityStatus = "RUNNING";
    }

    if (deltaMagnitude > FALL_SEVERE_THRESHOLD)
    {
        activityStatus = "SEVERE FALL DETECTED!";
        SerialMonitorInterface.println("Severe fall detected!");
        sendFallStatus("SEVERE FALL DETECTED\n");
        initiateFallDisplay();
    }
    else if (deltaMagnitude > FALL_MODERATE_THRESHOLD)
    {
        activityStatus = "MODERATE FALL DETECTED!";
        SerialMonitorInterface.println("Moderate fall detected!");
        sendFallStatus("MODERATE FALL DETECTED\n");
        initiateFallDisplay();
    }
    else if (deltaMagnitude > FALL_MINOR_THRESHOLD)
    {
        activityStatus = "MINOR FALL DETECTED!";
        SerialMonitorInterface.println("Minor fall detected!");
        sendFallStatus("MINOR FALL DETECTED\n");
        initiateFallDisplay();
    }
    else
    {
        sendFallStatus("SAFE\n");
    }

    // Check if BMA250 is not detected
    if (x == -1 && y == -1 && z == -1)
    {
        // Print error message to Serial Monitor
        SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    else
    {
        showSerial();
    }
}

/**
 * @brief Initiates the fall display by setting flags and timestamps.
 *
 */
void initiateFallDisplay()
{
    if (!fallDetectedFlag)
    {
        fallDetectedFlag = true;
        fallDetectedTime = millis();
        currentDisplayState = FALL_DISPLAY;
    }
}