#ifndef DATASENDER_H
#define DATASENDER_H

#include <Arduino.h>

void sendDataToServer(const String &fallStatus, const String &activityStatus, float temperature);

#endif
