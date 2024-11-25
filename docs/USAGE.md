# Usage Guide

## Prerequisites

- Ensure you have completed the installation steps outlined in the [Installation Guide](docs/INSTALLATION.md).
- Make sure your TinyZero device is connected to your computer via USB for programming.
- Confirm that your Flask server is deployed and running on PythonAnywhere.

## Table of Contents

1. [Uploading Firmware to the TinyZero](#1-uploading-firmware-to-the-tinyzero)
2. [Accessing the Flask Server on PythonAnywhere](#2-accessing-the-flask-server-on-pythonanywhere)
3. [Using the Care Link Dashboard](#3-using-the-care-link-dashboard)
4. [Interacting with the Device](#4-interacting-with-the-device)
5. [Troubleshooting](#5-troubleshooting)

## 1. Uploading Firmware to the TinyZero

### Step 1: Open the Arduino Project

- Open Visual Studio Code.
- Navigate to the `arduino/` directory of the cloned repository.
- Open the main Arduino project file, typically named `arduino.ino`.

### Step 2: Connect the TinyZero

- Connect your TinyZero board to your computer using a USB cable.
- Ensure the device is recognized by your system.

### Step 3: Select the Board and Port in VS Code

- Press `Ctrl+Shift+P` to open the Command Palette.
- Type `Arduino: Select Board` and choose **TinyZero**.
- Type `Arduino: Select Serial Port` and select the port associated with the TinyZero.

### Step 4: Install Required Libraries (If Not Already Installed)

- Ensure all required libraries are installed as per the [Installation Guide](docs/INSTALLATION.md).

### Step 5: Compile and Upload the Code

- Press `Ctrl+Shift+P` and select `Arduino: Verify` to compile the code.
- If the compilation is successful, select `Arduino: Upload` to upload the firmware to the TinyZero.
- Monitor the output for any errors and confirm a successful upload.

## 2. Accessing the Flask Server on PythonAnywhere

Since the Flask server is deployed on PythonAnywhere, you don't need to run it locally. Follow these steps to ensure everything is set up correctly and to interact with the server.

### Step 1: Verify Server Deployment

- Log in to your [PythonAnywhere](https://www.pythonanywhere.com/) account.
- Ensure that your web app for the Care Link project is set up and running.
- The server should be accessible at:

  ```link
  https://<your-pythonanywhere-username>.pythonanywhere.com
  ```

### Step 2: Manage Server Settings (If Needed)

- **Reload the Web App**: After making changes to your code, navigate to the **Web** tab in the PythonAnywhere dashboard and click the **Reload** button to apply the updates.
- **Check Logs**: If you encounter issues, review the **Error log** and **Server log** in the **Web** tab for detailed error messages.

## 3. Using the Care Link Dashboard

### Accessing the Dashboard

- Open a web browser and navigate to your server URL:

- **PythonAnywhere**: `https://<your-pythonanywhere-username>.pythonanywhere.com`

### Exploring the Dashboard

- **Real-Time Data**: View real-time sensor data from the device, including:
  - Fall Status
  - Activity Status
  - Temperature
  - Medication Status
  - Panic Status

- **Medication Schedule**: Review and manage the current medication schedule.

## 4. Interacting with the Device

### Fall Detection

- Simulate a fall with the TinyZero device to trigger fall detection.
- The device will send data to the server, and the dashboard will update the fall status accordingly.

### Medication Reminders

- The device will display medication reminders based on the schedule.
- Use the device buttons to confirm or cancel medication intake.
- Your response will be sent to the server and reflected on the dashboard.

### Panic Button

- Press and hold the designated panic button on the device.
- An emergency alert will be sent to the server, updating the dashboard and notifying caregivers if configured.

### Temperature Monitoring

- The device continuously monitors ambient temperature.
- Significant changes or threshold breaches will trigger alerts on the dashboard.

## 5. Troubleshooting

### Device Not Connecting

- Ensure the TinyZero is properly connected via USB.
- Check that the correct board and serial port are selected in VS Code.
- Verify that the firmware has been successfully uploaded to the device.

### Upload Errors

- Verify that all required libraries are installed.
- Review any error messages in the output panel and refer to the [Installation Guide](docs/INSTALLATION.md) for library installation steps.

### Server Issues on PythonAnywhere

- **Web App Not Responding**:
- Ensure the web app is reloaded after recent changes.
- Check the **Error log** for any server-side errors.

- **Data Not Updating on Dashboard**:
- Confirm that the device is successfully sending data to the server.
- Check network connectivity between the device and the PythonAnywhere server.
- Review server logs for any errors or exceptions.

### Data Transmission Issues

- **No Data on Dashboard**:
- Ensure the device is powered on and connected to the network.
- Verify that the server URL in the device firmware points to your PythonAnywhere server.
- Check the serial monitor on the device for any error messages related to data transmission.

### General Tips

- Regularly sync your code with the repository to incorporate the latest changes.
- Monitor the serial output from the TinyZero for real-time debugging information.
- Consult the [Installation Guide](docs/INSTALLATION.md) for reinstallation or setup steps if necessary.
- Utilize PythonAnywhere's support resources and [Help Pages](https://help.pythonanywhere.com/) for additional assistance.

