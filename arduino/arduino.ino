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
#include "include/panicButton.h"                 // Include Panic Button module
#include <Arduino.h>


#include "include/wifi_module.h"                  // Include Medication module
#include "include/wifi_config.h"                  // Include Medication module


// Global variables
String medStatus = "MED_CONFIRM"; // Initialize with a default status

void setup()
{
    // Initialize serial communication
    SerialMonitorInterface.begin(9600);
    while (!SerialMonitorInterface);

    SerialMonitorInterface.println("Serial monitor initialized!");
    initializeWiFi();

    if (WiFi.status() == WL_CONNECTED) {
        SerialMonitorInterface.println("WiFi connected successfully.");
    } else {
        SerialMonitorInterface.println("WiFi connection failed.");
    }

    // Initialize the display
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

        sendAllSensorData(activityStatus.c_str(), activityStatus.c_str(), temp, getCurrentTime().c_str(), medStatus.c_str());
        if (currentScreen == MEDICATION_SCREEN)
        {
            handleMedicationConfirmation();
        } else {
            // Medication screen is not active; trigger panic button action
            panicButton();
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

    if (SerialMonitorInterface.available())
    {
        handleSerialInput();
    }

    
}