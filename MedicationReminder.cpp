#include "MedicationReminder.h"
#include "DisplayHandler.h"
#include "ButtonHandler.h"

RTC_DS3231* rtc;
struct Medication {
    String name;
    int hour;
    int minute;
    bool taken;
};

const int MAX_MEDICATIONS = 10;
Medication medications[MAX_MEDICATIONS];
int medicationCount = 0;

void initMedicationReminder(RTC_DS3231* rtcModule) {
    rtc = rtcModule;
    // Add medications here
    addMedication("Medication A", 9, 0);
    addMedication("Medication B", 12, 30);
    addMedication("Medication C", 18, 45);
}

void addMedication(String name, int hour, int minute) {
    if (medicationCount < MAX_MEDICATIONS) {
        medications[medicationCount].name = name;
        medications[medicationCount].hour = hour;
        medications[medicationCount].minute = minute;
        medications[medicationCount].taken = false;
        medicationCount++;
    }
}

void checkMedicationReminder() {
    DateTime now = rtc->now();
    int currentHour = now.hour();
    int currentMinute = now.minute();

    for (int i = 0; i < medicationCount; i++) {
        if (!medications[i].taken && currentHour == medications[i].hour && currentMinute == medications[i].minute) {
            displayReminder(medications[i].name.c_str());
            triggerAlert();

            bool taken = waitForAcknowledgment();
            medications[i].taken = taken;
            stopAlert();

            sendMedicationAcknowledgment(medications[i].name.c_str(), taken);
        }
    }

    if (currentHour == 0 && currentMinute == 0) {
        resetMedications();
    }
}

bool waitForAcknowledgment() {
    unsigned long startTime = millis();
    while (millis() - startTime < 60000) { // Wait for 1 minute
        if (buttonPressed()) {
            displayAcknowledgment();
            return true;
        }
    }
    return false;
}

void sendMedicationAcknowledgment(const char* medicationName, bool taken) {
    String status = taken ? "TAKEN" : "NOT_TAKEN";
    String message = "MEDICATION_ACK:" + String(medicationName) + ":" + status;
    sendToBluetooth(message.c_str());
}

void resetMedications() {
    for (int i = 0; i < medicationCount; i++) {
        medications[i].taken = false;
    }
}