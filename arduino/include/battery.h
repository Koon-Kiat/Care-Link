#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

float getVCC();
float getBattVoltage();
void displayBattery();
void lowBatteryAlert();

extern bool lowBatteryAlertShown;

#endif