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

    if (buttons & TSButtonLowerLeft)
    {
        if (currentScreen == MEDICATION_SCREEN) {
            // Confirm medication taken
            sendMedicationStatus("MED_CONFIRM");
            activityStatus = "MEDICATION CONFIRMED";
            display.clearScreen();
            display.setFont(liberationSansNarrow_10ptFontInfo);

            // Center "Confirmed!" message
            const char* message = "Confirmed!";
            int messageWidth = display.getPrintWidth(const_cast<char*>(message));
            int messageX = (SCREEN_WIDTH - messageWidth) / 2;
            int messageY = (SCREEN_HEIGHT - display.getFontHeight()) / 2;
            display.setCursor(messageX, messageY);
            display.print(message);

            delay(2000); // Display message for 2 seconds
            // Return to home screen
            currentScreen = HOME_SCREEN;
            alarmHandled = true;
            currentMedication = ""; // Reset currentMedication
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

        // Center "Not Taken!" message
        const char* message = "Not Taken!";
        int messageWidth = display.getPrintWidth(const_cast<char*>(message));
        int messageX = (SCREEN_WIDTH - messageWidth) / 2;
        int messageY = (SCREEN_HEIGHT - display.getFontHeight()) / 2;
        display.setCursor(messageX, messageY);
        display.print(message);

        delay(2000); // Display message for 2 seconds
        // Return to home screen
        currentScreen = HOME_SCREEN;
        alarmHandled = true;
        currentMedication = ""; // Reset currentMedication
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
    String currentTime = getCurrentTime(); // Gets time in "HH:MM" format

    for (const auto &med : medicationSchedule)
    {
        if (currentTime == med.time && !alarmHandled)
        {
            if (currentScreen != MEDICATION_SCREEN)
            {
                currentScreen = MEDICATION_SCREEN;
                currentMedication = med.type;
            }
        }
    }
}
