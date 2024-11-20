#ifndef FALLANDTEMPERATURESENSOR_H
#define FALLANDTEMPERATURESENSOR_H

#include <Arduino.h>
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

void checkFallDetectionAndTemperature();
void initiateFallDisplay();

#endif