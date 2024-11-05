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
        // Display "Help!" message
        display.clearScreen();
        display.setFont(thinPixel7_10ptFontInfo);
        display.setCursor(0, 0);
        display.print("Help!");
        delay(2000);

        // Send help status to the server
        sendPanicStatus("HELP: REQUESTED");
    }
}