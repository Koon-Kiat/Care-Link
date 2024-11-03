#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"

extern TinyScreen display;

void displayActivityStatus(const char *status, double temperature);
String getTemperatureCategory(double temperature);

#endif