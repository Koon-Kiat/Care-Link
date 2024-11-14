#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>
#include <WiFi101.h>
#include <ArduinoJson.h>                         // Include ArduinoJson for JSON handling

#include "include/BMA250.h"                      // Include BMA250 accelerometer module
#include "include/status.h"                      // Include StatusSender module
#include "include/fall_and_temperature_sensor.h" // Include FallAndTemperatureSensor module
#include "include/display.h"                     // Include Display module
#include "include/serial.h"                      // Include Serial module
#include "include/config.h"                      // Include configuration file
#include "include/medication.h"                  // Include Medication module
#include "include/WiFiModule.h"                  // Include WiFi module
#include "include/WiFiConfig.h"
#include "include/dataSender.h"

void setup()
{
    SerialMonitorInterface.begin(9600);
        while (!SerialMonitorInterface) {
        delay(100); // Ensure Serial is ready
    }


    initializeWiFi();

    Wire.begin();
    display.begin();
    display.setBrightness(10);

    // Initialize the RTC
    rtc.begin();

    // Initialize the BMA250 accelerometer
    SerialMonitorInterface.print("Initializing BMA...\n");
    accel_sensor.begin(BMA250_range_16g, BMA250_update_time_64ms);
}

void loop()
{
    unsigned long currentMillis = millis();
    // Sensor reading
    if (currentMillis - previousLoopTime >= SENSOR_READ_INTERVAL)
    {
        previousLoopTime = currentMillis;

        checkFallDetectionAndTemperature();
        if (currentScreen == MEDICATION_SCREEN)
        {
            handleMedicationConfirmation();
        }
    }

    // Medication alarm check
    checkMedicationAlarm();

    // Display updates
    static unsigned long lastDisplayUpdate = 0;
    if (currentMillis - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL)
    {
        lastDisplayUpdate = currentMillis;
        updateFallDisplayStatus();
    }


    // Retrieve the latest sensor data
    String activityStatus = getActivityStatus();   // Get activity status
    String fallStatus = getFallStatus();           // Get fall status
    float temperature = getTemperature();          // Get temperature

    // Send data to the server
    sendDataToServer(fallStatus, activityStatus, temperature);

    if (SerialMonitorInterface.available())
    {
        handleSerialInput();
    }
}
