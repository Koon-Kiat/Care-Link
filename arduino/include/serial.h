#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>
#include "uart.h"         // For SerialMonitorInterface
#include "status.h" // For sendFallStatus and sendTemperatureStatus
#include "display.h"      // For display functions (optional)

// Declare external variables defined in Care-Link.ino
extern int x;
extern int y;
extern int z;
extern double temp;

// Function Declarations

/**
 * @brief Handles serial input from the Serial Monitor.
 *        Responds to specific commands like 's' and 'q'.
 */
void handleSerialInput();

/**
 * @brief Displays sensor values on the Serial Monitor.
 */
void showSerial();

#endif 