#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"
#include "medication.h"

extern TinyScreen display;
enum ScreenState
{
    HOME_SCREEN,
    FALL_AND_TEMP_SCREEN,   // Activity Status Screen
    MEDICATION_INFO_SCREEN, // Add new screen state
    MEDICATION_SCREEN
};

extern ScreenState currentScreen;
extern ScreenState previousScreen;
extern String activityStatus;
extern String medStatus;
extern String panicStatus;


void displayHomeScreen();
void updateDisplay(double temperature, const char *activityStatus);
void updateFallDisplayStatus();
void displayTemperatureAndFallStatus(const char *status, double temperature);
String getTemperatureCategory(double temperature);
String getCurrentTime();
void displayMedicationScreen();
void displayMedicationInfoScreen();

#endif