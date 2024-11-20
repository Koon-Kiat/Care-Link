#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

float getVCC();
float getBattVoltage();
void displayBattery();
void lowBatteryAlert();
float getSmoothedBattVoltage();

extern bool lowBatteryAlertShown;
extern double alpha;

#endif