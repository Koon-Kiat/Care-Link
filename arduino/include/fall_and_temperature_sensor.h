#ifndef FALLANDTEMPERATURESENSOR_H
#define FALLANDTEMPERATURESENSOR_H

#include <Arduino.h>
#include "bma250.h" 
#include "status.h" 
#include "config.h"

extern TinyScreen display;
extern BMA250 accel_sensor;
extern int x, y, z;
extern double temp;

// Function declarations
void checkFallDetectionAndTemperature();
String getFallStatus();
String getActivityStatus();
float getTemperature();

#endif