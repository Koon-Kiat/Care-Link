# Usage Guide

## 1. Verify and Upload

   - Verify your code in the Arduino IDE or VS Code and upload it to the TinyZero.

## 2. Run Serial Monitor to Start BT

   - Open the Serial Monitor in the Arduino IDE or VS Code to start the Bluetooth communication.
   - You should see the following output in the Serial Monitor:
     ```bash
     BLE Stack Initialized.
     UART service added successfully.
     Initializing BMA...
     General Discoverable Mode.
     ```

## 3. Stop Serial Monitor

   - Once the Bluetooth communication is established, stop the Serial Monitor.

## 4. List Avaliable Bluetooth Devices

   - Run the following script to list available Bluetooth devices and their MAC addresses:
     ```bash
     python mac_address.py
     ```
   - You should see output similar to this:
     ```bash
     Device: None, Address: **:**:**:**:**:**
     Device: None, Address: **:**:**:**:**:**
     Device: None, Address: **:**:**:**:**:**
     Device: None, Address: **:**:**:**:**:**
     Device: None, Address: **:**:**:**:**:**
     Device: BlueNRG, Address: **:**:**:**:**:**
     Device: None, Address: **:**:**:**:**:**
     Device: None, Address: **:**:**:**:**:**z
     ```
   - Identify and note the MAC address of the `BlueNRG` device.

## 5. Fetch Bluetooth Services and Characteristics

   - Run the following script to fetch the services and characteristics of the Bluetooth device:
     ```bash
     python uuid_checker.py
     ```
   - You should see output similar to this:
     ```bash
     Service: ****1801-****-****-****-********34fb
         Characteristic: ****2a05-****-****-****-********34fb
     Service: ****1800-****-****-****-********34fb
         Characteristic: ****2a00-****-****-****-********34fb
         Characteristic: ****2a01-****-****-****-********34fb
         Characteristic: ****2a04-****-****-****-********34fb
     Service: ****0001-****-****-****-********cca9e
         Characteristic: ****0002-****-****-****-********cca9e
         Characteristic: ****0003-****-****-****-********cca9e
     ```
   - Choose the third last line and the last line for the `ALERT_SERVICE_UUID` and `ALERT_CHARACTERISTIC_UUID` environment variables respectively.

## 6. Run the Telegram Bot

   - Run your Python script directly from the terminal:
     ```bash
     python telegram_bot.py
     ```

## 7. Interact with the Telegram Bot
   - Open Telegram and start a chat with your bot using the provided token. Send the `/start` command to begin interaction.