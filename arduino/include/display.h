#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"
#include "medication.h" 

extern TinyScreen display;

// Add screen state enumeration
enum ScreenState
{
    HOME_SCREEN,
    ACTIVITY_SCREEN
};

extern ScreenState currentScreen;

// Declare new display functions
void displayHomeScreen();
void updateDisplay(double temperature, const char *activityStatus);
void displayActivityStatus(const char *status, double temperature);
String getTemperatureCategory(double temperature);

// Add extern declaration for activityStatus
extern String activityStatus;

#endif