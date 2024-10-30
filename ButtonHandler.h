#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

#define BUTTON_PIN 2  // Define the pin number for the button

void initButtonHandler();
bool buttonPressed();

#endif