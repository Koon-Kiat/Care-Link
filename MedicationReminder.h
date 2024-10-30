#ifndef MEDICATION_REMINDER_H
#define MEDICATION_REMINDER_H

#include <RTClib.h>

void initMedicationReminder(RTC_DS3231* rtc);
void checkMedicationReminder();
void sendMedicationAcknowledgment(const char* medicationName, bool taken);

#endif