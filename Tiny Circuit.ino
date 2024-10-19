#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <Wire.h>

//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


uint8_t ble_rx_buffer[201];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

TinyScreen display = TinyScreen(TinyScreenDefault);

void setup() {
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
  BLEsetup();

  Wire.begin();
  display.begin();
  display.setBrightness(10);
}


void loop() {
  aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
  if (ble_rx_buffer_len) {//Check if data is available
    SerialMonitorInterface.print(ble_rx_buffer_len);
    SerialMonitorInterface.print(" : ");
    SerialMonitorInterface.println((char*)ble_rx_buffer);

    display.clearScreen(); // Clear the screen
    display.setFont(thinPixel7_10ptFontInfo); // Set the font
    int width = display.getPrintWidth((char*)ble_rx_buffer); // Get the width of the message
    display.setCursor(64 - width / 2, 32 - 10 / 2); // Center the message on the screen
    display.fontColor(TS_8b_Green, TS_8b_Black); // Set the font color
    display.print((char*)ble_rx_buffer); // Display the received message on the screen
    display.println(); // Move to the next line

    ble_rx_buffer_len = 0;//clear afer reading
  }
  if (SerialMonitorInterface.available()) {//Check if serial input is available to send
    delay(10);//should catch input
    uint8_t sendBuffer[201];
    uint8_t sendLength = 0;
    while (SerialMonitorInterface.available() && sendLength < 199) {
      sendBuffer[sendLength] = SerialMonitorInterface.read();
      sendLength++;
    }
    if (SerialMonitorInterface.available()) {
      SerialMonitorInterface.print(F("Input truncated, dropped: "));
      if (SerialMonitorInterface.available()) {
        SerialMonitorInterface.write(SerialMonitorInterface.read());
      }
    }
    sendBuffer[sendLength] = '\0'; //Terminate string
    sendLength++;
    if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer, sendLength))
    {
      SerialMonitorInterface.println(F("TX dropped!"));
    } else {
      SerialMonitorInterface.print(F("Sent: "));
      SerialMonitorInterface.println((char*)sendBuffer); // Optional: log the sent message to Serial Monitor
      
    }
  }
}

void onReceiveData(uint8_t* data, uint8_t length) {
  if (length <= sizeof(ble_rx_buffer)) {
    memcpy(ble_rx_buffer, data, length);
    ble_rx_buffer_len = length; // Update the length of the received message
  }
}


