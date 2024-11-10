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
    // Read accelerometer data
    accel_sensor.read();
    x = accel_sensor.X;
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

    // Check if BMA250 is not detected
    if (x == -1 && y == -1 && z == -1)
    {
        SerialMonitorInterface.println("ERROR! NO BMA250 DETECTED!");
        return;
    }

    // Calculate magnitude of acceleration vector
    double magnitude = sqrt(x * x + y * y + z * z);
    static double prevMagnitude = magnitude;
    double deltaMagnitude = fabs(magnitude - prevMagnitude);
    prevMagnitude = magnitude;

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100)
    {
        return;
    }
    lastCheckTime = currentTime;

    // Debug output
    SerialMonitorInterface.print("x: ");
    SerialMonitorInterface.print(x);
    SerialMonitorInterface.print(" y: ");
    SerialMonitorInterface.print(y);
    SerialMonitorInterface.print(" z: ");
    SerialMonitorInterface.println(z);

    SerialMonitorInterface.print("magnitude: ");
    SerialMonitorInterface.println(magnitude);

    SerialMonitorInterface.print("deltaMagnitude: ");
    SerialMonitorInterface.println(deltaMagnitude);

    // Define thresholds for activity detection (adjusted based on observations)
    const double RESTING_MAX_THRESHOLD = 3.0;
    const double WALKING_MAX_THRESHOLD = 100.0;
    const double RUNNING_MAX_THRESHOLD = 270.0;

    // Activity detection based on magnitude_no_gravity
    if (fallDetectedFlag)
    {
        return; // Skip updating activity status during fall display
    }
    else if (deltaMagnitude <= RESTING_MAX_THRESHOLD)
    {
        activityStatus = "RESTING";
    }
    else if (deltaMagnitude > RESTING_MAX_THRESHOLD && deltaMagnitude <= WALKING_MAX_THRESHOLD)
    {
        activityStatus = "WALKING";
    }
    else if (deltaMagnitude > WALKING_MAX_THRESHOLD && deltaMagnitude <= RUNNING_MAX_THRESHOLD)
    {
        activityStatus = "RUNNING";
    }

    // Define fall thresholds for categorization (adjusted based on observations)
    const double FALL_MINOR_MIN_THRESHOLD = 280.0;
    const double FALL_MINOR_MAX_THRESHOLD = 600.0;
    const double FALL_MODERATE_MAX_THRESHOLD = 800.0;

    // Fall detection based on deltaMagnitude
    if (deltaMagnitude > FALL_MODERATE_MAX_THRESHOLD)
    {
        activityStatus = "SEVERE FALL DETECTED!";
        SerialMonitorInterface.println("Severe fall detected!");
        sendFallStatus("SEVERE FALL DETECTED\n");
        initiateFallDisplay();
    }
    else if (deltaMagnitude > FALL_MINOR_MAX_THRESHOLD && deltaMagnitude <= FALL_MODERATE_MAX_THRESHOLD)
    {
        activityStatus = "MODERATE FALL DETECTED!";
        SerialMonitorInterface.println("Moderate fall detected!");
        sendFallStatus("MODERATE FALL DETECTED\n");
        initiateFallDisplay();
    }
    else if (deltaMagnitude > FALL_MINOR_MIN_THRESHOLD && deltaMagnitude <= FALL_MINOR_MAX_THRESHOLD)
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

    // Show serial debug information
    showSerial();
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
    }
}