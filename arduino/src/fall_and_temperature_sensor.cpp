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
    const int16_t FALL_MINOR_THRESHOLD_LOW = -400;
    const int16_t FALL_MODERATE_THRESHOLD_LOW = -1000;
    const int16_t FALL_SEVERE_THRESHOLD = -1500;

    const int16_t FALL_MINOR_THRESHOLD_HIGH = 400;     
    const int16_t FALL_MODERATE_THRESHOLD_HIGH = 1000; 
    const int16_t FALL_SEVERE_THRESHOLD_HIGH = 1500;   

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100)
    {
        return; 
    }
    lastCheckTime = currentTime;

    // Define thresholds for activity detection
    const double RESTING_THRESHOLD = 248.0; 
    const double WALKING_THRESHOLD = 260.0; 
    const double RUNNING_THRESHOLD = 300.0; 

    // Calculate magnitude
    double magnitude = sqrt(x * x + y * y + z * z);

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

    if (z <= FALL_SEVERE_THRESHOLD || z >= FALL_SEVERE_THRESHOLD_HIGH)
    {
        activityStatus = "SEVERE FALL DETECTED!";
        SerialMonitorInterface.println("Severe fall detected!");
        sendFallStatus("SEVERE FALL DETECTED\n");
        initiateFallDisplay();
    }
    else if (z <= FALL_MODERATE_THRESHOLD_LOW || z >= FALL_MODERATE_THRESHOLD_HIGH)
    {
        activityStatus = "MODERATE FALL DETECTED!";
        SerialMonitorInterface.println("Moderate fall detected!");
        sendFallStatus("MODERATE FALL DETECTED\n");
        initiateFallDisplay();
    }
    else if (z <= FALL_MINOR_THRESHOLD_LOW || z >= FALL_MINOR_THRESHOLD_HIGH)
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