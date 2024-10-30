// BLE.h
#ifndef BLE_H
#define BLE_H

#include <SPI.h>
#include <STBLE.h>
#include "UART.h"

// Define Bluetooth-related constants and variables
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

extern char receivedBuffer[MAX_RECEIVED_MESSAGE_SIZE];
extern uint8_t receivedBufferLength;
extern bool ble_connection_state;
extern uint8_t ble_rx_buffer_len;

// **Add the following line**
extern uint8_t ble_rx_buffer[MAX_BUFFER_SIZE];

// Initialize Bluetooth
void BluetoothSetup();

// Process Bluetooth events
void BluetoothLoop();

// Send data over Bluetooth
void sendToBluetooth(const char *data);

#endif // BLE_H