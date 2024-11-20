#ifndef MEDICATION_H
#define MEDICATION_H


#include <Arduino.h>

struct MedicationSchedule
{
    String type;
    String time;
};

extern std::vector<MedicationSchedule> medicationSchedule;
extern String currentMedication;


void showMedicationReminder(int cursorX, int cursorY);
void handleMedicationConfirmation();
void checkMedicationAlarm();

#endif
