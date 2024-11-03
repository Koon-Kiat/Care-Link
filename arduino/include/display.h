// src/Display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"

// Declare the external display object defined in Care-Link.ino
extern TinyScreen display;

void displayActivityStatus(const char *status, double temperature);
String getTemperatureCategory(double temperature);

#endif