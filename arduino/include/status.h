#ifndef STATUS_SENDER_H
#define STATUS_SENDER_H

#include <Arduino.h>

// Undefine min and max macros
#undef min
#undef max

#include "config.h"

void sendFallStatus(const char *status);
void sendTemperatureStatus(double temperature);
void sendStatus(const char *message);
void sendMedicationStatus(const char *status);
void sendPanicStatus(const char *panicStatus);

#endif