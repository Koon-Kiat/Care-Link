#ifndef STATUS_SENDER_H
#define STATUS_SENDER_H

#include <Arduino.h>

// Undefine min and max macros
#undef min
#undef max

#include "wifi_module.h"
#include "config.h"

void sendFallStatus(const char *status);
void sendTemperatureStatus(double temperature);
void sendStatus(const char *message);
void sendMedicationStatus(const char *status);
void sendPanicStatus(const char *panicStatus);
void sendMedicationStatus(const char *medStatus); 
void sendAllSensorData(const char *fallStatus, const char *activityStatus, double temperature, const char *timestamp);


#endif