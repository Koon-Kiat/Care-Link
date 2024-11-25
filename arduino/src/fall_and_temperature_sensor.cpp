#include "../include/fall_and_temperature_sensor.h"
#include "../include/bma250.h"
#include "../include/display.h"
#include "../include/serial.h"
#include <math.h>

/**
 * @brief Checks for fall detection and temperature readings.
 *
 * This function reads the accelerometer data from the BMA250 sensor and calculates the acceleration magnitude.
 * It then calculates the jerk (rate of change of acceleration) and compares it to predefined thresholds to detect falls.
 * If a fall is detected, it sends the fall status to the server and initiates the fall display.
 * It also updates the activity status based on the acceleration magnitude.
 *
 */
void checkFallDetectionAndTemperature()
{
    // Read accelerometer data
    accel_sensor.read();
    double x = accel_sensor.X;
    double y = accel_sensor.Y;
    double z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

    // Check for sensor error
    if (x == -1 && y == -1 && z == -1)
    {
        SerialMonitorInterface.println("ERROR! NO BMA250 DETECTED!");
        return;
    }

    // Time calculations for jerk
    static unsigned long lastTime = 0;
    unsigned long currentTime = millis();
    double deltaTime = (currentTime - lastTime) / 1000.0; // Convert ms to seconds
    if (deltaTime == 0)
    {
        return;
    }
    lastTime = currentTime;

    // Previous acceleration values
    static double prevX = x, prevY = y, prevZ = z;

    // Calculate acceleration magnitude
    double magnitude = sqrt(x * x + y * y + z * z);

    // Calculate jerk (rate of change of acceleration)
    double jerkX = (x - prevX) / deltaTime;
    double jerkY = (y - prevY) / deltaTime;
    double jerkZ = (z - prevZ) / deltaTime;
    double totalJerk = sqrt(jerkX * jerkX + jerkY * jerkY + jerkZ * jerkZ);

    // Update previous acceleration values
    prevX = x;
    prevY = y;
    prevZ = z;

    // Define thresholds with consideration of rest magnitude (~33)
    const double REST_MAGNITUDE = 33.0;                                  // Average magnitude at rest
    const double FREE_FALL_DELTA = 10.0;                                  // Reduced drop from rest for sensitive detection
    const double FREE_FALL_THRESHOLD = REST_MAGNITUDE - FREE_FALL_DELTA; // 23.0
    const double IMPACT_THRESHOLD_MINOR = 25.0;                          // Increased for minor impacts
    const double IMPACT_THRESHOLD_MODERATE = 140.0;                      // Increased for moderate impacts
    const double IMPACT_THRESHOLD_SEVERE = 400.0;                        // Increased for severe impacts
    const double JERK_THRESHOLD = 60.0;

    // Show serial debug information
    SerialMonitorInterface.print("Magnitude: ");
    SerialMonitorInterface.println(magnitude);
    SerialMonitorInterface.print("Total jerk: ");
    SerialMonitorInterface.println(totalJerk);
    SerialMonitorInterface.print("X: ");
    SerialMonitorInterface.print(x);
    SerialMonitorInterface.print(", Y: ");
    SerialMonitorInterface.print(y);
    SerialMonitorInterface.print(", Z: ");
    SerialMonitorInterface.println(z);

    // Fall detection logic
    static bool freeFallDetected = false;
    if (magnitude < FREE_FALL_THRESHOLD)
    {
        freeFallDetected = true;
    }

    // Check for fall based on magnitude and jerk thresholds
    else if (freeFallDetected && magnitude > IMPACT_THRESHOLD_MINOR && totalJerk > JERK_THRESHOLD)
    {
        if (magnitude <= IMPACT_THRESHOLD_MODERATE)
        {
            activityStatus = "MINOR FALL DETECTED!";
            SerialMonitorInterface.println("Minor fall detected!");
            sendFallStatus("MINOR FALL DETECTED\n");
        }
        else if (magnitude <= IMPACT_THRESHOLD_SEVERE)
        {
            activityStatus = "MODERATE FALL DETECTED!";
            SerialMonitorInterface.println("Moderate fall detected!");
            sendFallStatus("MODERATE FALL DETECTED\n");
        }
        else
        {
            activityStatus = "SEVERE FALL DETECTED!";
            SerialMonitorInterface.println("Severe fall detected!");
            sendFallStatus("SEVERE FALL DETECTED\n");
        }
        initiateFallDisplay();
        freeFallDetected = false;
    }
    else
    {
        freeFallDetected = false;
    }

    // Update activity status based on magnitude
    if (!fallDetectedFlag)
    {
        // Calculate the change in magnitude
        static double prevMagnitude = magnitude;
        double deltaMagnitude = fabs(magnitude - prevMagnitude);
        prevMagnitude = magnitude;

        SerialMonitorInterface.print("Delta Magnitude: ");
        SerialMonitorInterface.println(deltaMagnitude);

        // Define thresholds for activity detection based on delta magnitude
        const double RESTING_MAX_DELTA = 3.0;
        const double WALKING_MAX_DELTA = 25.0;

        if (deltaMagnitude < RESTING_MAX_DELTA)
        {
            activityStatus = "RESTING";
        }
        else if (deltaMagnitude < WALKING_MAX_DELTA)
        {
            activityStatus = "WALKING";
        }
        else
        {
            activityStatus = "RUNNING";
        }

        // Show serial debug information
        showSerial();
    }
}

/**
 * @brief Initiates the fall display.
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