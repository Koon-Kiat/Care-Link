#include "../include/config.h"
#include "../include/medication.h"

// Define the actual global variables
char receivedBuffer[MAX_RECEIVED_MESSAGE_SIZE];
uint8_t receivedBufferLength = 0;
bool ble_connection_state = false;
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_rx_buffer[MAX_BUFFER_SIZE];

// Define the TinyScreen display object
TinyScreen display = TinyScreen(TinyScreenDefault);

// Define the BMA250 accelerometer instance
BMA250 accel_sensor;

// Define sensor data variables
int x = 0, y = 0, z = 0;
double temp = 0.0;

// Define the RTC object
RTCZero rtc; // create an rtc object

/* Change these values to set the current initial time */
const byte hours = 22;
const byte minutes = 14;
const byte seconds = 0;

/* Change these values to set the current initial date */
const byte day = 8;
const byte month = 11;
const byte year = 24;

// Define the screen width and height 
const int SCREEN_WIDTH = 96;
const int SCREEN_HEIGHT = 64;

// Define fall detection variables
bool fallDetectedFlag = false;
unsigned long fallDetectedTime = 0;
const unsigned long SENSOR_READ_INTERVAL = 500;    
const unsigned long DISPLAY_UPDATE_INTERVAL = 1000; 
unsigned long previousLoopTime = 0;

// Define the alarmHandled variable
bool alarmHandled = false;

// Define color constant
const uint8_t TS_8b_Orange = 0x13;

// Initialize medication schedule
std::vector<MedicationSchedule> medicationSchedule = {
    {"Aspirin", "08:00"},
    {"Vitamin D", "12:00"},
    {"Metformin", "22:14"}
};
String currentMedication = "";

// Define the panic button state
bool isPanicOn = false;

// Sleep and display variables
unsigned long sleepTimer = 0;
uint8_t displayOn = 0;
uint8_t buttonReleased = 1;
int sleepTimeout = 10;