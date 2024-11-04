// UART.h
#ifndef UART_H
#define UART_H

#include <Arduino.h>
#include <stdint.h>
#include "config.h"

// ----------------------
// Macro Definitions
// ----------------------
#if BLE_DEBUG
#include <stdio.h>
extern char sprintbuff[100];
#define PRINTF(...)                               \
    {                                             \
        sprintf(sprintbuff, __VA_ARGS__);         \
        SerialMonitorInterface.print(sprintbuff); \
    }
#else
#define PRINTF(...)
#endif


// ----------------------
// External Variable Declarations
// ----------------------
extern uint8_t ble_rx_buffer_len;
extern volatile uint8_t set_connectable;
extern uint16_t connection_handle;
extern bool connected;

// ----------------------
// Function Declarations
// ----------------------
#ifdef __cplusplus
extern "C"
{
#endif

    // Only HCI_Event_CB needs C linkage
    void HCI_Event_CB(void *pckt);

#ifdef __cplusplus
}
#endif

// UART Functions (C++ linkage)
void UARTSetup();
void UARTSend(const char *message);

// BLE Functions (C++ linkage)
int BLEsetup();
void aci_loop();

// Additional Functions (C++ linkage)
void Attribute_Modified_CB(uint16_t handle, uint8_t data_length, uint8_t *att_data);
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);
void setConnectable(void);

// Data Sending Function (C++ linkage)
uint8_t lib_aci_send_data(uint8_t ignore, uint8_t *sendBuffer, uint8_t sendLength);

// UART Transmission Function (C++ linkage)
uint8_t Write_UART_TX(char *TXdata, uint8_t datasize);

#endif