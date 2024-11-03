#ifndef STATUS_SENDER_H
#define STATUS_SENDER_H

#include <Arduino.h>
#include "config.h"

void sendFallStatus(const char *status);
void sendTemperatureStatus(double temperature);
void sendStatus(const char *message);

#endif