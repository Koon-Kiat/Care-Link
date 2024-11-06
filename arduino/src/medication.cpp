#include "../include/medication.h"
#include "../include/display.h"
#include "../include/config.h"
#include "../include/status.h"

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
    // Read button states using display.getButtons()
    uint8_t buttons = display.getButtons();

    if (buttons & TSButtonUpperRight)
    {
        // Confirm medication taken
        sendMedicationStatus("MED_CONFIRM");
        activityStatus = "CONFIRMED";
        delay(2000); // Display message for 2 seconds
    }
    else if (buttons & TSButtonLowerRight)
    {
        // Handle cancellation if needed
        sendMedicationStatus("MED_CANCEL");
        activityStatus = "CANCELLED";
        delay(2000); // Display message for 2 seconds
    }
}
