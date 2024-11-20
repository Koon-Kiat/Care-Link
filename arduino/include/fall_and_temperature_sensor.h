#ifndef FALLANDTEMPERATURESENSOR_H
#define FALLANDTEMPERATURESENSOR_H

#include <Arduino.h>

// Undefine min and max macros
#undef min
#undef max

#include <vector>
#include "bma250.h"
#include "status.h"
#include "config.h"

extern TinyScreen display;
extern BMA250 accel_sensor;
extern int x, y, z;
extern double temp;

// Function declarations
void checkFallDetectionAndTemperature();
void initiateFallDisplay();
String getFallStatus();
String getActivityStatus();
float getTemperature();

#endif