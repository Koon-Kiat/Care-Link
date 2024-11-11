#ifndef MEDICATION_H
#define MEDICATION_H

// ...existing code...

#include <Arduino.h>

// Undefine min and max macros
#undef min
#undef max

#include <vector>

// ...existing code...

struct MedicationSchedule
{
    String type;
    String time;
};

extern std::vector<MedicationSchedule> medicationSchedule;
extern String currentMedication;

// ...existing code...

void showMedicationReminder(int cursorX, int cursorY);
void handleMedicationConfirmation();
void checkMedicationAlarm();

#endif
