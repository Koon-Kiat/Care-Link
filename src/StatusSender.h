#ifndef STATUS_SENDER_H
#define STATUS_SENDER_H

#include <Arduino.h>

// Send fall status
void sendFallStatus(const char *status);

// Send temperature status
void sendTemperatureStatus(double temperature);

#endif // STATUS_SENDER_H