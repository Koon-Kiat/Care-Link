# Care Link

![GitHub repo size](https://img.shields.io/github/repo-size/Koon-Kiat/Care-Link)
![GitHub commit activity](https://img.shields.io/github/commit-activity/t/Koon-Kiat/Care-Link?style=flat)
![GitHub issues](https://img.shields.io/github/issues/Koon-Kiat/Care-Link)
![GitHub Issues or Pull Requests](https://img.shields.io/github/issues-pr/Koon-Kiat/Care-Link)
![GitHub Repo stars](https://img.shields.io/github/stars/Koon-Kiat/Care-Link?style=flat)

## Overview

The Care Link Project aims to create a portable, wearable device designed to assist individuals with cognitive disabilities, including those with mental disabilities and dementia. This innovative device facilitates medication management, provides daily reminders, and ensures safety through real-time alerts and monitoring. Leveraging advanced components such as TinyZero, TinyShield BT, and TinyScreen, the project delivers a compact, efficient, and user-friendly solution for enhancing the quality of life for its users.

## Components

- **TinyZero**: Model ASM2021-R
- **TinyScreen**: Model ASD2431-R
- **TinyShield BT**: Model ASD2116-R
- **TinyShield WiFi Board**: Model ASD2123-R

## Key Technologies

- **C++**: Primary programming language for the firmware.
- **Python**: Used for telegram bot.
- **Flask**: A lightweight web application framework used to build the server and RESTful APIs.
- **Arduino IDE and CLI**: For developing and uploading firmware to the hardware devices.
- **Arduino Extension for VS Code**: Integration of Arduino development within VS Code.
- **Arduino Libraries**:
  - **SPI**: Serial Peripheral Interface communication.
  - **Wire**: I2C communication between devices.
  - **STBLE**: Bluetooth Low Energy communication.
  - **WiFi101**: Wi-Fi connectivity for the Arduino WiFi Shield 101 and MKR1000.
  - **Arduino**: Core Arduino functions and definitions.
  - **TinyScreen**: Control of the TinyScreen display.
  - **ArduinoJson**: JSON serialization and deserialization.
  
## Features

- **Fall Detection**: Automatically detects falls and immediately sends alerts to caregivers, ensuring prompt assistance in case of emergencies.
- **Ambient Temperature Monitoring**: Continuously tracks environmental temperature, helping prevent heat-related illnesses and ensuring comfort.
- **Medication Management**: Provides reminders for scheduled medication, assisting users in adhering to their prescriptions.
- **Panic Button**: A manual panic button allows users to send an emergency alert instantly to caregivers, providing added security and peace of mind.
- **Battery Indicator**: Displays current battery levels, notifying both users and caregivers when the device needs to be recharged, ensuring continuous operation.
- **Care Link Website Dashboard**: An online platform accessible to caregivers, displaying all real-time data for centralized monitoring.

## Installation

For detailed installation instructions, please refer to the [Installation Guide](docs/INSTALLATION.md).

## Usage

After completing the installation, refer to the [Usage Guide](docs/USAGE.md) for detailed instructions on how to use the Tiny Circuit Project, including running the code, interacting with the Telegram bot, and troubleshooting common issues.
