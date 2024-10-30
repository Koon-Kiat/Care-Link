// src/SensorHandler.h
#ifndef FALLANDTEMPERATURESENSOR_H
#define FALLANDTEMPERATURESENSOR_H

#include <Arduino.h>
#include "BMA250.h"       // Accelerometer module
#include "StatusSender.h" // For sendFallStatus and sendTemperatureStatus
#include "TinyScreen.h"   // For displayActivityStatus and showSerial

extern TinyScreen display;
extern BMA250 accel_sensor;
extern int x, y, z;
extern double temp;

// Function declaration
void checkFallDetectionAndTemperature();

#endif // FALLANDTEMPERATURESENSOR_H