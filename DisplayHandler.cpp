#include "DisplayHandler.h"

TinyScreen* display;

void initDisplayHandler(TinyScreen* disp) {
    display = disp;
}

void displayReminder(const char* medicationName) {
    display->clearScreen();
    display->setCursor(0, 0);
    display->fontColor(TS_8b_White, TS_8b_Black);
    display->print("Take Medication:");
    display->setCursor(0, 16);
    display->print(medicationName);
    display->update();
}

void displayAcknowledgment() {
    display->clearScreen();
    display->setCursor(0, 0);
    display->fontColor(TS_8b_White, TS_8b_Black);
    display->print("Acknowledged");
    display->update();
}