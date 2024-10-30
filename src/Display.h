// src/Display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TinyScreen.h>

// Declare the external display object defined in Care-Link.ino
extern TinyScreen display;

// Function Declarations

/**
 * @brief Displays the activity status and temperature on the TinyScreen.
 *
 * @param status      The activity status string (e.g., "RESTING", "WALKING", "RUNNING", "FALL DETECTED!").
 * @param temperature The current temperature in Celsius.
 */
void displayActivityStatus(const char *status, double temperature);

/**
 * @brief Categorizes the temperature.
 *
 * @param temperature The current temperature in Celsius.
 * @return String The temperature category ("COOL", "COMFORTABLE", "HOT").
 */
String getTemperatureCategory(double temperature);

#endif // DISPLAY_H