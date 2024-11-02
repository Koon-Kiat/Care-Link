#ifndef FALLANDTEMPERATURESENSOR_H
#define FALLANDTEMPERATURESENSOR_H

#include <Arduino.h>
#include "bma250.h"       // Accelerometer module
#include "status.h" // For sendFallStatus and sendTemperatureStatus
#include "config.h"

extern TinyScreen display;
extern BMA250 accel_sensor;
extern int x, y, z;
extern double temp;

void checkFallDetectionAndTemperature();

#endif