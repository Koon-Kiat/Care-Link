#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

extern bool lowBatteryAlertShown;
extern double alpha;
extern string batteryStatus;

float getVCC();
float getBattVoltage();
void displayBattery();
void lowBatteryAlert();
float getSmoothedBattVoltage();

#endif