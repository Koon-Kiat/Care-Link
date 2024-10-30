#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>
#include "src/BMA250.h"                   // Include BMA250 accelerometer module
#include "src/UART.h"                     // Include UART module
#include "src/BLE.h"                      // Include BLE module
#include "src/StatusSender.h"             // Include StatusSender module
#include "src/FallAndTemperatureSensor.h" // Include FallAndTemperatureSensor module
#include "src/Display.h"                  // Include Display module
#include "src/Serial.h"                   // Include Serial module

// Define BMA250 I2C address
#define BMA250_ADDRESS 0x18

// Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

// Define the display object (only once)
TinyScreen display = TinyScreen(TinyScreenDefault);

// Declare global variables (defined once here)
BMA250 accel_sensor; // Instance of BMA250 accelerometer
int x, y, z;
double temp;

void setup()
{
    UARTSetup();
    BLEsetup();

    Wire.begin();
    display.begin();
    display.setBrightness(10);

    // Initialize the BMA250 accelerometer with default range and bandwidth settings
    SerialMonitorInterface.print("Initializing BMA...\n");
    accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
}

void loop()
{
    aci_loop(); // Process any ACI commands or events from the NRF8001

    if (ble_connection_state)
    {

        // Check if data is available
        if (ble_rx_buffer_len)
        {
            SerialMonitorInterface.print(ble_rx_buffer_len);
            SerialMonitorInterface.print(" : ");
            SerialMonitorInterface.println((char *)ble_rx_buffer);

            // Display received message
            display.clearScreen();
            display.setFont(thinPixel7_10ptFontInfo);
            int width = display.getPrintWidth((char *)ble_rx_buffer);
            display.setCursor(64 - width / 2, 32 - 10 / 2);
            display.fontColor(TS_8b_Green, TS_8b_Black);
            display.print((char *)ble_rx_buffer);
            display.println();

            ble_rx_buffer_len = 0; // Clear after reading
        }

        // Check accelerometer for fall detection and temperature
        checkFallDetectionAndTemperature();
    }
    // Check if serial input is available
    if (SerialMonitorInterface.available())
    {
        handleSerialInput();
    }
}
