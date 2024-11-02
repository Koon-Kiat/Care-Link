#include "../include/config.h"

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
