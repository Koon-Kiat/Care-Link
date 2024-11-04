#include "../include/display.h"
#include "../include/medication.h"

/**
 * @brief Updates and displays the activity status and temperature on the TinyScreen.
 *
 * This function clears the screen and sets the font to display the current activity status
 * and temperature. It categorizes the temperature and provides a medication reminder
 * to the user to confirm their medication intake.
 *
 * @param status The current activity status (e.g., "RESTING", "WALKING", "RUNNING", "FALL DETECTED!").
 * @param temperature The current temperature in Celsius.
 */
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
    cursorY += lineHeight;

    // Print Medication Reminder in two lines
    display.setCursor(cursorX, cursorY);
    display.print("Medication:");
    cursorY += lineHeight;
    display.setCursor(cursorX, cursorY);
    display.print("Press to confirm.");
}

/**
 * @brief Categorizes the temperature.
 *
 * @param temperature The current temperature in Celsius.
 * @return String The temperature category ("C" for COOL, "N" for NORMAL, "H" for HOT).
 */
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