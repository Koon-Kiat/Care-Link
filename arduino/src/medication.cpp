#include "../include/medication.h"
#include "../include/display.h"
#include "../include/config.h"
#include "../include/status.h" // Added include for Status module

/**
 * @brief Handles user confirmation for medication intake.
 *
 * This function reads the button states using `display.getButtons()` to determine
 * whether the user has confirmed taking their medication or canceled the confirmation.
 * Based on the button pressed, it updates the display with the corresponding message,
 * sends the appropriate status to the server, and updates the activity status.
 */
void handleMedicationConfirmation()
{
    // Read button states using display.getButtons()
    uint8_t buttons = display.getButtons();

    if (buttons & TSButtonUpperRight)
    {
        // Confirm medication taken
        display.clearScreen();
        display.setFont(thinPixel7_10ptFontInfo);
        display.setCursor(0, 0);
        display.print("Taken");
        // Send confirmation status
        sendStatus("MED_CONFIRM");
        delay(2000); // Display message for 2 seconds
        displayActivityStatus("CONFIRMED", temp);
    }
    else if (buttons & TSButtonLowerRight)
    {
        // Handle cancellation if needed
        display.clearScreen();
        display.setFont(thinPixel7_10ptFontInfo);
        display.setCursor(0, 0);
        display.print("Not Taken.");
        // Send cancellation status
        sendStatus("MED_CANCEL");
        delay(2000); // Display message for 2 seconds
        displayActivityStatus("CANCELLED", temp);
    }
}
