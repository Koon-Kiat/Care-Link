#include "../include/panicButton.h"
#include "../include/display.h"
#include "../include/status.h"

void panicButton()
{
    // Read button states using display.getButtons()
    uint8_t buttons = display.getButtons();

    // Check if the bottom left button is pressed
    if (buttons & TSButtonLowerLeft)
    {
        display.clearScreen();

        display.setFont(liberationSansNarrow_16ptFontInfo);
        display.fontColor(TS_8b_Red, TS_8b_Black);

        const char *header = "Help!";
        int headerWidth = display.getPrintWidth(const_cast<char *>(header));
        int headerX = (SCREEN_WIDTH - headerWidth) / 2;
        display.setCursor(headerX, 0);
        display.print(header);

        delay(5000); // 5 seconds on screen

        // Send help status to the server
        sendPanicStatus("HELP: REQUESTED");
    }
}