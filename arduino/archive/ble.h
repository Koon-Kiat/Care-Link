#ifndef BLE_H
#define BLE_H

#include <SPI.h>
#include <STBLE.h>
#include "uart.h"
#include "config.h"


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

#endif