#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>
#include "include/BMA250.h"             // Include BMA250 accelerometer module
#include "include/UART.h"                 // Include UART module
#include "include/ble.h"                         // Include BLE module
#include "include/status.h"                      // Include StatusSender module
#include "include/fall_and_temperature_sensor.h" // Include FallAndTemperatureSensor module
#include "include/display.h"                     // Include Display module
#include "include/serial.h"                      // Include Serial module
#include "include/config.h"                      // Include configuration file

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
