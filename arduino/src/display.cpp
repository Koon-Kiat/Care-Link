#include "../include/display.h"


void displayActivityStatus(const char *status, double temperature)
{
    display.clearScreen();
    display.setFont(thinPixel7_10ptFontInfo);

    int cursorX = 0;
    int cursorY = 0;
    int lineHeight = display.getFontHeight() + 1;

    // Print "Status:" on the first line
    display.setCursor(cursorX, cursorY);
    display.fontColor(TS_8b_Green, TS_8b_Black);
    display.print("Status: ");
    display.print(status);
    cursorY += lineHeight;

    // Print temperature value and short category
    display.setCursor(cursorX, cursorY);
    display.print("Temp/Cat: ");
    display.print(temperature);
    display.print("/");
    display.print(getTemperatureCategory(temperature));
}


String getTemperatureCategory(double temperature)
{
    if (temperature < 28)
    {
        return "C"; // COOL
    }
    else if (temperature >= 28 && temperature <= 33)
    {
        return "N"; // NORMAL
    }
    else
    {
        return "H"; // HOT
    }
}