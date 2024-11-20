#ifndef STATUS_SENDER_H
#define STATUS_SENDER_H

#include <Arduino.h>
#include "wifi_module.h"
#include "config.h"

void sendFallStatus(const char *status);
void sendTemperatureStatus(double temperature);
void sendStatus(const char *message);
void sendMedicationStatus(const char *medStatus); 
void sendAllSensorData(const char *fallStatus, const char *activityStatus, double temperature, const char *timestamp);

void sendPanicStatus(const char *panicStatus);

#endif