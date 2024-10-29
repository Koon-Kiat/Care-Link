# Usage Guide

## Prerequisites

- Ensure you have completed the installation steps outlined in the [Installation Guide](docs/INSTALL.md).
- Make sure your TinyZero is connected to your computer via USB.

## 1. Verify and Upload

- Verify your code in the Arduino IDE or VS Code and upload it to the TinyZero.
- Ensure there are no errors during the upload process.

## 2. List Avaliable Bluetooth Devices

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

## 3. Fetch Bluetooth Services and Characteristics

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

## 4. Run the Telegram Bot

- Run your Python script directly from the terminal:
  ```bash
  python telegram_bot.py
  ```

## 5. Interact with the Telegram Bot

- Open Telegram and start a chat with your bot using the provided token. You **do not need to send the `/start` command**; the bot will automatically send a welcome message to you as soon as the script starts.

## Troubleshooting

- **Bluetooth Device Not Found**: Ensure your Bluetooth device is powered on and within range.
- **UUID Fetching Issues**: Verify that the correct MAC address is being used.
- **Telegram Bot Not Responding**: Check your internet connection and ensure the bot token is correct.
- **General Errors**: Refer to the TinyZero Setup Tutorial for additional troubleshooting tips.
