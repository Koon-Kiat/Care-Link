#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>
#include "uart.h"    // For SerialMonitorInterface
#include "status.h"  // For sendFallStatus and sendTemperatureStatus
#include "display.h" // For display functions (optional)

extern int x;
extern int y;
extern int z;
extern double temp;

void handleSerialInput();
void showSerial();

#endif