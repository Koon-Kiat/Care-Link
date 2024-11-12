#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <ArduinoJson.h>

String createJsonPayload(const String &fallStatus, const String &activityStatus, float temperature);

#endif
