#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"
#include "medication.h" // Added include for Medication module

extern TinyScreen display;

void displayActivityStatus(const char *status, double temperature);
String getTemperatureCategory(double temperature);

#endif