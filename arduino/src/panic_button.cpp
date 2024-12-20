#include "../include/panic_button.h"
#include "../include/display.h"
#include "../include/status.h"

/**
 * @brief Toggles the panic button state.
 * 
 * This function reads the button states using `display.getButtons()` to determine
 * whether the user has pressed the panic button. If the button is pressed, it toggles
 * the panic state and sends the appropriate status to the server.
 * 
 */
void panicButton()
{
    // Read button states using display.getButtons()
    uint8_t buttons = display.getButtons();

    // Check if the bottom left button is pressed
    if (buttons & TSButtonLowerLeft)
    {
        // Toggle the panic state
        isPanicOn = !isPanicOn;

        if (isPanicOn)
        {
            display.clearScreen();

            display.setFont(liberationSansNarrow_16ptFontInfo);
            display.fontColor(TS_8b_Red, TS_8b_Black);

            const char *header = "Help On";
            int headerWidth = display.getPrintWidth(const_cast<char *>(header));
            int headerX = (SCREEN_WIDTH - headerWidth) / 2;
            display.setCursor(headerX, 0);
            display.print(header);

            delay(5000);
            sendPanicStatus("HELP: REQUESTED");
            panicStatus = "HELP: REQUESTED";

        }
        else
        {
            // Clear the display to indicate the toggle is off
            display.clearScreen();
            display.setFont(liberationSansNarrow_16ptFontInfo);
            display.fontColor(TS_8b_Green, TS_8b_Black);

            const char *header = "Help Off";
            int headerWidth = display.getPrintWidth(const_cast<char *>(header));
            int headerX = (SCREEN_WIDTH - headerWidth) / 2;
            display.setCursor(headerX, 0);
            display.print(header);

            delay(5000);

            sendPanicStatus("HELP: CANCELLED");
            panicStatus = "HELP: CANCELLED";

        }

        // Add a short delay to debounce the button press
        delay(300);
    }
}
