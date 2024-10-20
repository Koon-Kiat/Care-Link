#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>

// Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

#define MAX_BUFFER_SIZE 256 // Increase buffer size to accommodate more data
uint8_t ble_rx_buffer[MAX_BUFFER_SIZE];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

#define MAX_RECEIVED_MESSAGE_SIZE 256
char receivedBuffer[MAX_RECEIVED_MESSAGE_SIZE];
uint8_t receivedBufferLength = 0;

TinyScreen display = TinyScreen(TinyScreenDefault);

void setup()
{
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface)
    ; // Block until a serial monitor is opened with TinyScreen+!
  BLEsetup();

  Wire.begin();
  display.begin();
  display.setBrightness(10);
}

void loop()
{
  aci_loop(); // Process any ACI commands or events from the NRF8001

  if (ble_rx_buffer_len)
  { // Check if data is available
    SerialMonitorInterface.print(ble_rx_buffer_len);
    SerialMonitorInterface.print(" : ");
    SerialMonitorInterface.println((char *)ble_rx_buffer);

    // Display received message
    display.clearScreen();
    display.setFont(thinPixel7_10ptFontInfo);
    int width = display.getPrintWidth((char *)ble_rx_buffer);
    display.setCursor(64 - width / 2, 32 - 10 / 2);
    display.fontColor(TS_8b_Green, TS_8b_Black);
    display.print((char *)ble_rx_buffer);
    display.println();

    ble_rx_buffer_len = 0; // Clear after reading
  }

  // Check if serial input is available
  if (SerialMonitorInterface.available())
  {
    delay(10);                        // Allow for a brief delay
    char sendBuffer[MAX_BUFFER_SIZE]; // Buffer to hold the data
    uint8_t sendLength = 0;

    // Read available data from serial until newline or max buffer size
    while (SerialMonitorInterface.available() && sendLength < (MAX_BUFFER_SIZE - 1))
    {
      char c = SerialMonitorInterface.read();
      if (c == '\n' || c == '\r')
        break; // Stop reading at newlines
      sendBuffer[sendLength++] = c;
    }

    // Null-terminate the string
    sendBuffer[sendLength] = '\0';

    // Log if input is truncated
    if (SerialMonitorInterface.available())
    {
      SerialMonitorInterface.print(F("Input truncated, dropped: "));
      while (SerialMonitorInterface.available())
      {
        SerialMonitorInterface.write(SerialMonitorInterface.read());
      }
    }

    // Send data in chunks of 20 bytes
    uint8_t offset = 0;
    while (offset < sendLength)
    {
      uint8_t chunkSize = min(sendLength - offset, 20); // Determine the chunk size
      if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t *)&sendBuffer[offset], chunkSize))
      {
        SerialMonitorInterface.println(F("TX dropped!"));
        break; // Stop sending if failed
      }
      offset += chunkSize; // Move to the next chunk

      // Log the sent chunk
      SerialMonitorInterface.print(F("Sent chunk: "));
      SerialMonitorInterface.write(&sendBuffer[offset - chunkSize], chunkSize);
      SerialMonitorInterface.println();
    }

    // Optionally send a termination character after all chunks
    char terminationChar = '\n'; // Use newline as termination
    if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t *)&terminationChar, 1))
    {
      SerialMonitorInterface.println(F("Termination character TX dropped!"));
    }
    else
    {
      SerialMonitorInterface.println(F("Termination character sent."));
    }

    SerialMonitorInterface.print(F("Full message sent: "));
    SerialMonitorInterface.println((char *)sendBuffer); // Log the sent message
  }
}

void onReceiveData(uint8_t* data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        // Append incoming data to the buffer
        if (receivedBufferLength < MAX_RECEIVED_MESSAGE_SIZE - 1) {
            receivedBuffer[receivedBufferLength++] = data[i];

            // Check for termination character
            if (data[i] == '\n') {
                // Null-terminate the string
                receivedBuffer[receivedBufferLength] = '\0';

                // Print the complete message
                SerialMonitorInterface.print(F("Complete message: "));
                SerialMonitorInterface.println(receivedBuffer);

                // Reset the buffer for the next message
                receivedBufferLength = 0;
            }
        } else {
            SerialMonitorInterface.println(F("Received message too long!"));
            receivedBufferLength = 0; // Reset the length
        }
    }
}
