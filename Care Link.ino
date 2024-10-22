#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>
#include "BMA250.h"

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

#define MAX_BUFFER_SIZE 256 // Increase buffer size to accommodate more data
uint8_t ble_rx_buffer[MAX_BUFFER_SIZE];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

#define MAX_RECEIVED_MESSAGE_SIZE 256
char receivedBuffer[MAX_RECEIVED_MESSAGE_SIZE];
uint8_t receivedBufferLength = 0;

TinyScreen display = TinyScreen(TinyScreenDefault);
BMA250 accel_sensor; // Create an instance of the BMA250 accelerometer
int x, y, z;
double temp;

void setup()
{
    SerialMonitorInterface.begin(9600);
    while (!SerialMonitorInterface)
        ; // Block until a serial monitor is opened with TinyScreen+!
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

    // Check if serial input is available
    if (SerialMonitorInterface.available())
    {
        handleSerialInput();
    }
}

void checkFallDetectionAndTemperature()
{
    accel_sensor.read(); // Use the read method to update acceleration values
    x = accel_sensor.X;  // Access X, Y, Z directly
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0); // Convert raw temperature to Celsius

    // Define fall thresholds for categorization
    const int16_t FALL_MINOR_THRESHOLD_LOW = -400;
    const int16_t FALL_MODERATE_THRESHOLD_LOW = -1000;
    const int16_t FALL_SEVERE_THRESHOLD = -1500;

    const int16_t FALL_MINOR_THRESHOLD_HIGH = 400;     // Positive threshold for minor rise
    const int16_t FALL_MODERATE_THRESHOLD_HIGH = 1000; // Positive threshold for moderate rise
    const int16_t FALL_SEVERE_THRESHOLD_HIGH = 1500;   // Positive threshold for severe rise

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100)
    {           // Check every 100ms
        return; // Skip this loop if we are still within the sampling period
    }
    lastCheckTime = currentTime; // Update the last check time

    // Define thresholds for activity detection
    const double RESTING_THRESHOLD = 248.0; // Adjusted based on observed resting magnitude
    const double WALKING_THRESHOLD = 260.0; // Adjusted based on testing
    const double RUNNING_THRESHOLD = 300.0; // Adjusted based on testing

    // Check activity status based on magnitude
    double magnitude = sqrt(x * x + y * y + z * z);
    String activityStatus;

    if (magnitude < RESTING_THRESHOLD)
    {
        activityStatus = "RESTING"; // Adjusted to indicate rest
    }
    else if (magnitude >= RESTING_THRESHOLD && magnitude < WALKING_THRESHOLD)
    {
        activityStatus = "WALKING";
    }
    else if (magnitude >= WALKING_THRESHOLD)
    {
        activityStatus = "RUNNING";
    }

    // Check thresholds for fall detection
    if (z <= FALL_SEVERE_THRESHOLD || z >= FALL_SEVERE_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Severe fall detected!");
        sendFallStatus("SEVERE FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else if (z <= FALL_MODERATE_THRESHOLD_LOW || z >= FALL_MODERATE_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Moderate fall detected!");
        sendFallStatus("MODERATE FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else if (z <= FALL_MINOR_THRESHOLD_LOW || z >= FALL_MINOR_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Minor fall detected!");
        sendFallStatus("MINOR FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else
    {
        sendFallStatus("SAFE\n");
    }

    // Display activity status and temperature on TinyScreen
    displayActivityStatus(activityStatus.c_str(), temp);
    sendTemperatureStatus(temp);
    if (x == -1 && y == -1 && z == -1)
    {
        // Print error message to Serial Monitor
        SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    else
    {
        showSerial();
    }

    delay(1000); // Delay for stability
}

void displayActivityStatus(const char *status, double temperature)
{
    display.clearScreen();
    display.setFont(thinPixel7_10ptFontInfo);

    int maxWidth = 96; // Set to your screen width in pixels (assuming 96px width for TinyScreen)
    int cursorX = 0;
    int cursorY = 0;

    // Print "Status:" on the first line
    display.setCursor(cursorX, cursorY);
    display.fontColor(TS_8b_Green, TS_8b_Black);
    display.print("Status: ");
    cursorX = display.getPrintWidth("Status: "); // Move the cursor after "Status: "

    // Print the activity status
    display.print(status);                  // Print the activity status
    cursorY += display.getFontHeight() + 2; // Move to the next line for buffer

    // Print a buffer line
    display.setCursor(0, cursorY);
    display.print(" ");                     // Empty line for buffer
    cursorY += display.getFontHeight() + 2; // Move to the next line for temperature display

    // Print "Temperature:" on the next line
    display.setCursor(0, cursorY);
    display.print("Temperature: ");
    cursorX = display.getPrintWidth("Temperature: "); // Move the cursor after "Temperature: "

    // Print the temperature value
    display.print(temperature);
    display.print(" C"); // Append Celsius indicator

    // Display current temperature category
    cursorY += display.getFontHeight() + 2; // Move to the next line for category display
    display.setCursor(0, cursorY);
    // display.print("Temp Category: ");
    display.print(getTemperatureCategory(temperature)); // Display temperature category
}

String getTemperatureCategory(double temperature)
{
    if (temperature < 28)
    {
        return "COOL";
    }
    else if (temperature >= 28 && temperature <= 33)
    {
        return "COMFORTABLE";
    }
    else // temperature > 25.0
    {
        return "HOT";
    }
}

void handleSerialInput()
{
    char c = SerialMonitorInterface.read();
    if (c == 's') // Use 's' for sending status updates
    {
        sendFallStatus("STATUS REQUESTED\n");
    }
    else if (c == 'q') // Use 'q' for quitting
    {
        SerialMonitorInterface.println("Quit requested!");
        while (1)
            ; // Hang the system for quitting
    }
}


void sendToBluetooth(const char *data) {
    if (ble_connection_state) {
        // Set the ignore variable appropriately (use 0 as a placeholder)
        uint8_t ignore = 0; 

        // Ensure the data fits in the buffer
        uint8_t sendLength = strlen(data);
        uint8_t sendBuffer[MAX_RECEIVED_MESSAGE_SIZE]; 

        // Check if the message is too long
        if (sendLength > sizeof(sendBuffer)) {
            SerialMonitorInterface.println("Error: Message too long!");
            return;
        }

        strncpy((char*)sendBuffer, data, sizeof(sendBuffer));

        // Log the data being sent
        SerialMonitorInterface.print("Sending data over BLE: ");
        SerialMonitorInterface.println(data);

        // Send the data
        uint8_t result = lib_aci_send_data(ignore, sendBuffer, sendLength);
        if (result != 0) {  // Assuming 0 is the success code
            SerialMonitorInterface.print("Failed to send data over BLE, error code: ");
            SerialMonitorInterface.println(result);
        }
    } else {
        SerialMonitorInterface.println("BLE connection is not established.");
    }
}



void sendFallStatus(const char* status) {
    String message;

    if (strcmp(status, "SAFE\n") == 0) {
        message = "FALL: S"; // SAFE
    } else if (strcmp(status, "SEVERE FALL DETECTED\n") == 0) {
        message = "FALL: SV"; // Severe Fall
    } else if (strcmp(status, "MODERATE FALL DETECTED\n") == 0) {
        message = "FALL: M"; // Moderate Fall
    } else if (strcmp(status, "MINOR FALL DETECTED\n") == 0) {
        message = "FALL: MI"; // Minor Fall
    }

    SerialMonitorInterface.print("Preparing to send: ");
    SerialMonitorInterface.println(message.c_str());
    sendToBluetooth(message.c_str());
}

void sendTemperatureStatus(double temperature) {
    String message = "TEMP: ";

    // Categorize the temperature into short codes
    if (temperature < 28) {
        message += "C"; // COOL
    } else if (temperature >= 28 && temperature <= 33) {
        message += "N"; // NORMAL
    } else {
        message += "H"; // HOT
    }

    sendToBluetooth(message.c_str());
}



void showSerial()
{
    // Print sensor values to Serial Monitor
    SerialMonitorInterface.print("X: ");
    SerialMonitorInterface.print(x);
    SerialMonitorInterface.print(", Y: ");
    SerialMonitorInterface.print(y);
    SerialMonitorInterface.print(", Z: ");
    SerialMonitorInterface.print(z);
    SerialMonitorInterface.print(", Temperature: ");
    SerialMonitorInterface.print(temp);
    SerialMonitorInterface.println(" C");
}
