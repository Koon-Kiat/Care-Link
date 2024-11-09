#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>
#include "include/BMA250.h"                      // Include BMA250 accelerometer module
#include "include/status.h"                      // Include StatusSender module
#include "include/fall_and_temperature_sensor.h" // Include FallAndTemperatureSensor module
#include "include/display.h"                     // Include Display module
#include "include/serial.h"                      // Include Serial module
#include "include/config.h"                      // Include configuration file
#include "include/medication.h"                  // Include Medication module


void setup()
{
    SerialMonitorInterface.begin(9600);
    delay(1000);

    Wire.begin();
    display.begin();
    display.setBrightness(10);

    rtc.begin();

    SerialMonitorInterface.print("Initializing BMA...\n");
    accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
}

void loop()
{
    unsigned long currentMillis = millis();

    // Handle sensor reading every SENSOR_READ_INTERVAL
    if (currentMillis - previousLoopTime >= SENSOR_READ_INTERVAL)
    {
        previousLoopTime = currentMillis;

        checkFallDetectionAndTemperature();

        if (currentScreen == MEDICATION_SCREEN)
        {
            handleMedicationConfirmation();
        }
    }

    // Handle display updates every DISPLAY_UPDATE_INTERVAL
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
