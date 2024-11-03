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

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    display.begin();
    display.setBrightness(10);

    // Initialize the BMA250 accelerometer with default range and bandwidth settings
    SerialMonitorInterface.print("Initializing BMA...\n");
    accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
}

void loop()
{
    checkFallDetectionAndTemperature();

    if (SerialMonitorInterface.available())
    {
        handleSerialInput();
    }
}
