# Installation Guide

This guide outlines the steps to set up your Care Link Project, including required libraries, installation steps, and configurations for the Flask server application.

## Arduino Setup

### Required Libraries

- **SPI**: For Serial Peripheral Interface communication.
- **Wire**: For I2C communication between devices.
- **STBLE**: For Bluetooth Low Energy communication.
- **WiFi101**: For Wi-Fi connectivity using the WiFi Shield 101 and MKR1000.
- **Arduino**: Core Arduino functions and definitions.
- **TinyScreen**: For TinyScreen display control.
- **ArduinoJson**: For JSON serialization and deserialization.

### Installation Steps

1. **Install Arduino IDE and CLI**

   - Download and install the Arduino IDE and CLI from the [official website](https://www.arduino.cc/en/software/).
   - Follow the setup instructions provided on the website.

2. **Install Required Libraries**

   - Open the Arduino IDE.
   - Navigate to `Sketch` > `Include Library` > `Manage Libraries`.
   - Install the following libraries by searching for them in the Library Manager:
     - **SPI**
     - **Wire**
     - **WiFi101**
     - **TinyScreen**
     - **ArduinoJson**
   - For the `STBLE` library:
     - Download it from the [TinyCircuits Bluetooth Low Energy TinyShield Tutorial](https://learn.tinycircuits.com/Communication/Bluetooth-Low-Energy_TinyShield_Tutorial/).
     - Follow the instructions to add it to your Arduino libraries.
   - For the `Arduino` core library:
     - This is included with the Arduino IDE by default.

3. **Set Up Arduino Extension for VS Code**

   - Download and install Visual Studio Code from the [official website](https://code.visualstudio.com/).
   - Open Visual Studio Code.
   - Go to the Extensions view by clicking the Extensions icon in the Activity Bar or pressing `Ctrl+Shift+X`.
   - Search for `Arduino` and install the extension provided by Microsoft.
   - After installation, open the Command Palette (`Ctrl+Shift+P`) and type `Arduino: Initialize` to configure the extension.
   - Set `arduino.useArduinoCli` to `true` in your VS Code settings.
   - Ensure both `arduino.path` and `arduino.commandPath` are empty or unset.
   - In the settings, add the following URL to `arduino.additionalUrls`:

     ```
     https://files.tinycircuits.com/ArduinoBoards/package_tinycircuits_index.json
     ```

   - Open the Command Palette (`Ctrl+Shift+P`), select `Arduino: Board Manager`, and install:
     - **TinyCircuits SAMD Boards**
     - **Arduino SAMD Boards (32-bits ARM Cortex-M0+)**
   - Select the correct serial port via `Arduino: Select Serial Port`.

4. **Clone the Project Repository**

   - Open a terminal and execute:

     ```bash
     git clone https://github.com/Koon-Kiat/Care-Link.git
     ```

5. **Open the Project in VS Code**

   - Navigate to the cloned repository:

     ```bash
     cd Care-Link
     ```

   - Open the project in VS Code.
   - Open the Arduino project located in the `arduino/` directory.
   - Verify and upload the code to your TinyZero board.
   - Ensure no errors occur during the upload process.

## Server Setup

### Deploying the Flask Server on PythonAnywhere

Follow these steps to deploy your Flask application on PythonAnywhere:

1. **Create a PythonAnywhere Account**

   - Go to [PythonAnywhere](https://www.pythonanywhere.com/) and sign up for a free account.
   - Verify your email address if required.

2. **Upload Your Code to PythonAnywhere**

   - **Option 1: Using Git**
     - If your code is hosted on GitHub, you can clone your repository directly on PythonAnywhere.
     - In the PythonAnywhere dashboard, open a **Bash console**.
     - Run the following commands, replacing `<your-github-username>` and `<your-repository>` with your details:

       ```bash
       git clone https://github.com/<your-github-username>/<your-repository>.git
       ```

     - Navigate to your project directory:

       ```bash
       cd <your-repository>/server
       ```

   - **Option 2: Uploading Files Manually**
     - Alternatively, you can upload your code files directly.
     - In the PythonAnywhere dashboard, go to the **Files** tab.
     - Navigate to your home directory and create a new directory, e.g., `Care-Link`.
     - Upload your project files into this directory.

3. **Create a Virtual Environment**

   - In the **Bash console**, ensure you're in the `server` directory:

     ```bash
     cd ~/Care-Link/server
     ```

   - Create a virtual environment:

     ```bash
     python -m venv venv
     ```

   - Activate the virtual environment:

     ```bash
     source venv/bin/activate
     ```

   - Install the required libraries:

     ```bash
     pip install -r requirements.txt
     ```

4. **Configure the Web App on PythonAnywhere**

   - In the PythonAnywhere dashboard, go to the **Web** tab.
   - Click **Add a new web app**.
   - Choose the following settings during the setup:
     - **Manual configuration** (since you already have a Flask app).
     - Select the appropriate Python version (e.g., Python 3.8).

5. **Set Up WSGI Configuration**

   - On the web app configuration page, find the **WSGI configuration file** link and click it to edit.
   - Replace the contents with the following, adjusting the paths as necessary:

     ```python
     import sys
     import os

     # Add your project directory to the system path
     project_home = '/home/<your-pythonanywhere-username>/Care-Link/server'
     if project_home not in sys.path:
         sys.path.insert(0, project_home)

     # Activate your virtual environment
     activate_this = os.path.join(project_home, 'venv', 'bin', 'activate_this.py')
     with open(activate_this) as file_:
         exec(file_.read(), dict(__file__=activate_this))

     # Import the Flask app
     from server import app as application
     ```

     - Replace `<your-pythonanywhere-username>` with your PythonAnywhere username.
     - Ensure the paths to your project directory and virtual environment are correct.

6. **Set Up Static Files**

   - If your Flask app serves static files, configure the static files URL and directory in the **Static files** section of the web app configuration page.
   - For example:

     - **URL:** `/static/`
     - **Directory:** `/home/<your-pythonanywhere-username>/Care-Link/server/static`

7. **Reload the Web App**

   - After making all changes, click the **Reload** button at the top of the web app configuration page.
   - Your Flask app should now be running at `https://<your-pythonanywhere-username>.pythonanywhere.com`.

8. **Testing the App**

   - Navigate to your web app's URL in a browser to verify it's working.
   - Ensure that your app is functioning as expected.

## Troubleshooting
- **ModuleNotFoundError**: Ensure your virtual environment is activated and all dependencies are installed.
- **Incorrect Paths**: Double-check the file paths in your WSGI configuration file.
- **App Not Loading**: Check the **Error log** in the **Web** tab for detailed error messages.
- **Static Files Not Displaying**: Confirm that the static files URL and directory are correctly set up in the **Static files** section.
- **General Errors**: Refer to the [PythonAnywhere Help Pages](https://help.pythonanywhere.com/) for additional assistance.
