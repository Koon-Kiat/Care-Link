#include "../include/medication.h"
#include "../include/display.h"
#include "../include/config.h"
#include "../include/status.h"
#include "../include/panicButton.h"


/**
 * @brief Handles user confirmation for medication intake.
 *
 * This function reads the button states using `display.getButtons()` to determine
 * whether the user has confirmed taking their medication or canceled the confirmation.
 * Based on the button pressed, it updates the display with the corresponding message,
 * sends the appropriate status to the server, and updates the activity status.
 * 
 */
void handleMedicationConfirmation()
{
    if (currentScreen != MEDICATION_SCREEN)
    {
        return;
    }

    // Read button states using display.getButtons()
    uint8_t buttons = display.getButtons();

    // if (buttons & TSButtonLowerLeft)
    // {
    //     // Confirm medication taken
    //     sendMedicationStatus("MED_CONFIRM");
    //     activityStatus = "MEDICATION CONFIRMED";
    //     display.clearScreen();
    //     display.setFont(liberationSansNarrow_10ptFontInfo);
    //     display.setCursor(10, 20);
    //     display.print("Confirmed!");
    //     delay(2000); // Display message for 2 seconds
    //     // Return to home screen
    //     currentScreen = HOME_SCREEN; // Changed from previousScreen to HOME_SCREEN
    //     alarmHandled = true;
    // }
    // else if (buttons & TSButtonLowerRight)
    // {
    //     // Handle cancellation if needed
    //     sendMedicationStatus("MED_CANCEL");
    //     activityStatus = "MEDICATION CANCELLED";
    //     display.clearScreen();
    //     display.setFont(liberationSansNarrow_10ptFontInfo);
    //     display.setCursor(10, 20);
    //     display.print("Not Taken!");
    //     delay(2000); // Display message for 2 seconds
    //     // Return to home screen
    //     currentScreen = HOME_SCREEN; // Changed from previousScreen to HOME_SCREEN
    //     alarmHandled = true;
    // }

    if (buttons & TSButtonLowerLeft)
    {
        if (currentScreen == MEDICATION_SCREEN) {
            // Confirm medication taken
            sendMedicationStatus("MED_CONFIRM");
            activityStatus = "MEDICATION CONFIRMED";
            display.clearScreen();
            display.setFont(liberationSansNarrow_10ptFontInfo);
            display.setCursor(10, 20);
            display.print("Confirmed!");
            delay(2000); // Display message for 2 seconds
            // Return to home screen
            currentScreen = HOME_SCREEN;
            alarmHandled = true;
        } else {
            // If not on medication screen, act as a panic button
            panicButton();
        }
    }
    else if (buttons & TSButtonLowerRight)
    {
        // Handle cancellation if needed
        sendMedicationStatus("MED_CANCEL");
        activityStatus = "MEDICATION CANCELLED";
        display.clearScreen();
        display.setFont(liberationSansNarrow_10ptFontInfo);
        display.setCursor(10, 20);
        display.print("Not Taken!");
        delay(2000); // Display message for 2 seconds
        // Return to home screen
        currentScreen = HOME_SCREEN;
        alarmHandled = true;
    }
}

/**
 * @brief Checks if it's time for medication and handles the alarm.
 *
 * This function compares the current time with the scheduled medication time.
 * If it's time for medication and the alarm hasn't been handled yet, it
 * updates the current screen to the medication screen.
 */
void checkMedicationAlarm()
{
    // Placeholder for medication time check
    String currentTime = getCurrentTime(); // Gets time in "HH:MM" format
    String medicationTime = "22:14";       // Example medication time

    if (currentTime == medicationTime && !alarmHandled)
    {
        if (currentScreen != MEDICATION_SCREEN)
        {
            currentScreen = MEDICATION_SCREEN;
        }
    }
}
