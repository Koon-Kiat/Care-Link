# Installtion Guide
This guide outlines the steps to set up your Tiny Circuit Project, including required libraries, installation steps, and configuration for a Telegram bot.

## Required Libraries

- **STBLE**: For Bluetooth Low Energy communication
- **BMA250**: For accelerometer functionality
- **TinyScreen**: For TinyScreen display control

## Installation Steps

1. **Install Arduino IDE IDE and CLI**
    Download and install the Arduino IDE and CLI from the [official website](https://www.arduino.cc/en/software/) and follow the setup instructions.

2. **Install Required Libraries**
   - Open the Arduino IDE, navigate to `Sketch` > `Include Library` > `Manage Libraries` and install `TinyScreen`
   - For `STBLE` library, follow the steps in the [TinyCircuits Bluetooth Low Energy TinyShield Tutorial](https://learn.tinycircuits.com/Communication/Bluetooth-Low-Energy_TinyShield_Tutorial/) to download the `STBLE` library from the provided link.

3. **Set Up Arduino Extension for VS Code**

   - Download and install Visual Studio Code from the [official website](https://code.visualstudio.com/).
   - Open Visual Studio Code.
   - Go to the Extensions view by clicking the Extensions icon in the Activity Bar or pressing `Ctrl+Shift+X`.
   - Search for `Arduino Community Edition` and install the extension by `vscode-arduino`.
   - After installation, open the Command Palette (`Ctrl+Shift+P`) and type `Arduino: Initialize` to configure the extension.
   - Set `arduino.useArduinoCli` to `true`, and ensure both `arduino.path` and `arduino.commandPath` are empty or unset.
   - In the settings, search for `arduino` and add the following URL to the Additional URLs: `https://files.tinycircuits.com/ArduinoBoards/package_tinycircuits_index.json`.
   - Ensure the option to use Arduino CLI is checked.
   - Open the Command Palette (`Ctrl+Shift+P`), search for `Arduino: Select Serial Port`, and choose the correct serial port.
   - Open the Command Palette (`Ctrl+Shift+P`), search for `TinyCircuits SAMD Boards` and `Arduino SAMD Boards (32-bits ARM Cortex-M0+)`, and install them.

4. **Clone the Project Repository**

   - Use the following command to clone the project repository:.
     ```bash
     git clone https://github.com/Koon-Kiat/Tiny-Circuit-Project
     ```

5. **Open the Project in VS Code**
   - Open the cloned project in VS Code, verify, and upload the code to your TinyZero.

6. **Upload the Code to the Microcontroller**:
   - Connect your TinyZero to your computer via USB.
   - Click the `Verify` button to verify the code.
   - Click the `Upload` button to upload the code to the TinyZero.

## General Troubleshooting Tips

Refer to the following tutorials if you encounter issues:

1. **TinyZero Setup Tutorial**

   - [TinyZero Setup Tutorial](https://learn.tinycircuits.com/Processors/TinyZero_Setup_Tutorial/)

2. **Bluetooth Low Energy TinyShield Tutorial**

   - [Bluetooth Low Energy TinyShield Tutorial](https://learn.tinycircuits.com/Communication/Bluetooth-Low-Energy_TinyShield_Tutorial/)

3. **TinyScreen TinyShield Tutorial**:
   - [TinyScreen TinyShield Tutorial](https://learn.tinycircuits.com/Display/TinyScreen_TinyShield_Tutorial/)

These resources provide comprehensive guides and troubleshooting tips to help you resolve common issues and ensure your project runs smoothly.


## Setting Up the Python Environment

1. **Install Python**
   - Download and install Python from the [official website](https://www.python.org/downloads/).

2. **Install Required Python Libraries**
   - Run the following command to install the required libraries globally:
     ```bash
     pip install logging os time asyncio bleak python-telegram-bot python-dotenv
     ```

## Creating a Telegram Bot

To create a Telegram bot and obtain the necessary credentials, follow these steps:

1. **Open Telegram**

   - Download and install the Telegram app on your device if you haven't already.
   - Open the Telegram app and log in to your account.

2. **Search for BotFather**

   - In the Telegram app, search for BotFather and start a chat with it. BotFather is the official bot for managing Telegram bots.

3. **Create a New Bot**

   - Send the command `/newbot` to BotFather.
   - Follow the prompts to choose a name and username for your bot. The username must end with `bot` (e.g., `MyAwesomeBot`).

4. **Obtain the Bot Token**

   - After creating the bot, BotFather will provide you with a token. This token is required to authenticate your bot and interact with the Telegram Bot API.
   - Copy the token and keep it secure. You will need it to configure your bot.

5. **Configure the Bot**:
   - Optionally, you can set a profile picture, description, and other settings for your bot using commands provided by BotFather.

## Configuring the Telegram Bot

1. **Create a `.env` File**

   - In the root of your project directory, create a file named `.env`.
   - Add the following environment variables to the `.env` file:
     ```bash
     TELEGRAM_TOKEN=your_telegram_bot_token
     TELEGRAM_CHAT_ID=your_telegram_chat_id
     BLE_DEVICE_MAC_ADDRESS=your_ble_device_mac_address
     ALERT_SERVICE_UUID=your_alert_service_uuid
     ALERT_CHARACTERISTIC_UUID=your_alert_characteristic_uuid
     ```

2. **Update the Python Script**:
   - Ensure your Python script is configured to load the environment variables and initialize the Telegram bot using the `python-telegram-bot` library.

## Usage

1. **Verify and Upload**

   - Verify your code in the Arduino IDE or VS Code and upload it to the TinyZero.

2. **Run Serial Monitor to Start BT**:

   - Open the Serial Monitor in the Arduino IDE or VS Code to start the Bluetooth communication.
   - You should see the following output in the Serial Monitor:
     ```bash
     BLE Stack Initialized.
     UART service added successfully.
     Initializing BMA...
     General Discoverable Mode.
     ```

3. **Stop Serial Monitor**

   - Once the Bluetooth communication is established, stop the Serial Monitor.

4. **Run the MAC Address Script**

   - Run the following script to list available Bluetooth devices and their MAC addresses:
     ```bash
     python ArduinoMac.py
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
     Device: None, Address: **:**:**:**:**:**
     ```
   - Identify and note the MAC address of the `BlueNRG` device.

5. **Run the UUID Checker Script**

   - Run the following script to fetch the services and characteristics of the Bluetooth device:
     ```bash
     python UUIDChecker.py
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

6. **Run the Telegram Bot Python File**

   - Run your Python script directly from the terminal:
     ```bash
     python TelegramBot.py
     ```

7. **Start Interaction with the Telegram Bot**
   - Open Telegram and start a chat with your bot using the provided token. Send the `/start` command to begin interaction.