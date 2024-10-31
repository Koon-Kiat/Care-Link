// src/Display.cpp
#include "Display.h"

#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

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
    else // temperature > 33
    {
        return "HOT";
    }
}