#include "ButtonHandler.h"

void initButtonHandler() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool buttonPressed() {
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW) {
        delay(50);  // Debounce delay
        if (digitalRead(BUTTON_PIN) == LOW) {
            while (digitalRead(BUTTON_PIN) == LOW) {
                delay(10);  // Wait for button release
            }
            return true;
        }
    }
    return false;
}