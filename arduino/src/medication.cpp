#include "../include/medication.h"
#include "../include/display.h"
#include "../include/config.h"
#include "../include/status.h"

extern ScreenState currentScreen;
extern ScreenState previousScreen; // Add this line

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
        // Confirm medication taken
        sendMedicationStatus("MED_CONFIRM");
        activityStatus = "MEDICATION CONFIRMED";
        display.clearScreen();
        display.setFont(liberationSansNarrow_10ptFontInfo);
        display.setCursor(10, 20);
        display.print("Confirmed!");
        delay(2000); // Display message for 2 seconds
        // Return to previous screen
        currentScreen = previousScreen;
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
        // Return to previous screen
        currentScreen = previousScreen;
    }
    else
    {
        // Handle unexpected button presses or no action
        // Optionally, provide feedback or keep the screen active
    }
}
