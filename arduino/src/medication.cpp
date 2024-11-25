#include "../include/medication.h"
#include "../include/display.h"
#include "../include/config.h"
#include "../include/status.h"
#include "../include/panic_button.h"

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
        if (currentScreen == MEDICATION_SCREEN)
        {
            // Confirm medication taken
            sendMedicationStatus("MED_CONFIRM");
            medStatus = "MEDICATION CONFIRMED";
            display.clearScreen();
            display.setFont(liberationSansNarrow_10ptFontInfo);

            // Center "Confirmed!" message
            const char *message = "Confirmed!";
            int messageWidth = display.getPrintWidth(const_cast<char *>(message));
            int messageX = (SCREEN_WIDTH - messageWidth) / 2;
            int messageY = (SCREEN_HEIGHT - display.getFontHeight()) / 2;
            display.setCursor(messageX, messageY);
            display.print(message);

            // Display confirmation message for 2 seconds
            delay(2000);

            // Return to home screen
            currentScreen = HOME_SCREEN;
            alarmHandled = true;
            currentMedication = ""; 
        }
        else
        {
            // If not on medication screen, act as a panic button
            panicButton();
        }
    }
    else if (buttons & TSButtonLowerRight)
    {
        // Handle cancellation if needed
        sendMedicationStatus("MED_CANCEL");
        medStatus = "MEDICATION CANCELLED";
        display.clearScreen();
        display.setFont(liberationSansNarrow_10ptFontInfo);

        // Center "Not Taken!" message
        const char *message = "Not Taken!";
        int messageWidth = display.getPrintWidth(const_cast<char *>(message));
        int messageX = (SCREEN_WIDTH - messageWidth) / 2;
        int messageY = (SCREEN_HEIGHT - display.getFontHeight()) / 2;
        display.setCursor(messageX, messageY);
        display.print(message);

        // Display cancellation message for 2 seconds
        delay(2000);

        // Return to home screen
        currentScreen = HOME_SCREEN;
        alarmHandled = true;
        currentMedication = "";
    }
}

/**
 * @brief Checks the current time against the medication schedule.
 * 
 */
void checkMedicationAlarm()
{
    String currentTime = getCurrentTime();

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

/**
 * @brief Prints the current medication schedule to the Serial Monitor.
 * 
 */
void printMedicationSchedule()
{
    Serial.println("Current Medication Schedule:");
    for (const auto &med : medicationSchedule)
    {
        Serial.print("Medication: ");
        Serial.print(med.type);
        Serial.print(", Time: ");
        Serial.println(med.time);
    }
}