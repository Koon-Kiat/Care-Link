#include "../include/panicButton.h"
#include "../include/display.h"
#include "../include/status.h"

// void panicButton()
// {
//     // Display "Help!" message
//     display.clearScreen();
//     display.setFont(thinPixel7_10ptFontInfo);
//     display.setCursor(0, 0);
//     display.print("Help!");
//     delay(2000);

//     // Send help status to the server
//     sendPanicStatus("HELP: REQUESTED");
// }

void panicButton()
{
    // Read button states using display.getButtons()
    uint8_t buttons = display.getButtons();

    // Check if the bottom left button is pressed
    if (buttons & TSButtonLowerLeft)
    {
        // // Display "Help!" message
        // display.clearScreen();
        // display.setFont(thinPixel7_10ptFontInfo);
        // display.setCursor(0, 0);
        // display.print("Help!");
        // delay(2000);
        display.clearScreen();

        display.setFont(liberationSansNarrow_16ptFontInfo);
        display.fontColor(TS_8b_Red, TS_8b_Black);

        const char *header = "Help!";
        int headerWidth = display.getPrintWidth(const_cast<char *>(header));
        int headerX = (SCREEN_WIDTH - headerWidth) / 2;
        display.setCursor(headerX, 0);
        display.print(header);

        // Send help status to the server
        sendPanicStatus("HELP: REQUESTED");
    }
}