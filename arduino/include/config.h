#ifndef CONFIG_H
#define CONFIG_H

#include <TinyScreen.h>
#include "bma250.h" // Include BMA250 accelerometer module

// Define Bluetooth-related constants
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

// Define BMA250 I2C address
#define BMA250_ADDRESS 0x18

// Debug output
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

#define MAX_BUFFER_SIZE 256
#define MAX_RECEIVED_MESSAGE_SIZE 256

// Declare global variables as extern
extern char receivedBuffer[MAX_RECEIVED_MESSAGE_SIZE];
extern uint8_t receivedBufferLength;
extern bool ble_connection_state;
extern uint8_t ble_rx_buffer_len;
extern uint8_t ble_rx_buffer[MAX_BUFFER_SIZE];

// Declare display object as extern
extern TinyScreen display;

// Declare the accelerometer sensor as extern
extern BMA250 accel_sensor;

// Declare sensor data variables as extern
extern int x, y, z;
extern double temp;

#endif // CONFIG_H
