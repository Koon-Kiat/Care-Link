#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>
#include "uart.h"   
#include "status.h"  
#include "display.h"

extern int x;
extern int y;
extern int z;
extern double temp;

void handleSerialInput();
void showSerial();

#endif