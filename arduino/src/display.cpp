#include "../include/display.h"
#include "../include/medication.h"
#include "../include/battery.h"

ScreenState currentScreen = HOME_SCREEN;
ScreenState previousScreen = HOME_SCREEN; 
String activityStatus = "RESTING";

// uint32_t sleepTime = 0;
unsigned long sleepTimer = 0;
uint8_t displayOn = 0;
uint8_t buttonReleased = 1;
int sleepTimeout = 10; // 10 seconds until display sleeps

/**
 * @brief Displays the home screen on the TinyScreen.
 *
 * This function clears the screen and displays the home screen with the current time and a welcome message.
 * It also provides navigation arrows for the user to switch between screens.
 *
 */
void displayHomeScreen()
{
    display.clearScreen();

    // Set font for arrows
    display.setFont(liberationSansNarrow_8ptFontInfo); // Use a smaller font
    display.fontColor(TS_8b_White, TS_8b_Black);

    // Define padding
    const int arrowPadding = 2;

    // Define arrow characters
    char leftArrow[] = "<";
    char rightArrow[] = ">";

    // Get the actual width of the arrow characters
    int arrowWidth = display.getPrintWidth(leftArrow); // Assuming both arrows have the same width

    // Display navigation arrows with padding
    display.setCursor(arrowPadding, 0);
    display.print(leftArrow); // Upper left arrow

    display.setCursor(SCREEN_WIDTH - arrowWidth - arrowPadding, 0);
    display.print(rightArrow); // Upper right arrow

    // Adjust line height based on font size
    int lineHeight = display.getFontHeight() + 2;

    // Display time in a slightly larger font for better visibility
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);

    String timeStr = getCurrentTime(); // Format "HH:MM"

    // Convert String to char array
    char timeStrC[6]; // "HH:MM" + null terminator
    timeStr.toCharArray(timeStrC, sizeof(timeStrC));

    // Get the width of the time string
    int timeWidth = display.getPrintWidth(timeStrC);

    // Calculate available width between arrows
    int availableWidth = SCREEN_WIDTH - (arrowWidth * 2) - (arrowPadding * 2);

    // Check if time fits within the available width
    if (timeWidth > availableWidth)
    {
        // Handle overflow: truncate if necessary
        timeStrC[5] = '\0'; // Ensure null termination
        timeWidth = display.getPrintWidth(timeStrC);
    }

    // Center the time between the arrows and align it vertically with the arrows
    int timeX = arrowPadding + arrowWidth + ((availableWidth - timeWidth) / 2);
    int timeY = (display.getFontHeight() < 8) ? 2 : 1; // Adjust y-position to align with arrows
    display.setCursor(timeX, timeY);
    display.print(timeStrC);

    // Reset font and color for the rest of the text
    display.setFont(liberationSansNarrow_8ptFontInfo); // Use the same font for consistency
    display.fontColor(TS_8b_White, TS_8b_Black);

    // Update line height after font change
    lineHeight = display.getFontHeight() + 2;
    int cursorY = display.getFontHeight() + 14; // Start below the time display

    // Helper lambda to center and print text
    auto centerPrint = [&](const String &text, int &y) -> void
    {
        const int maxLength = 32; // Maximum expected length
        char textC[maxLength];
        text.toCharArray(textC, sizeof(textC));

        // Get the width of the text
        int textWidth = display.getPrintWidth(textC);

        // If text width exceeds screen width, truncate the text
        if (textWidth > SCREEN_WIDTH)
        {
            // Estimate maximum characters that can fit
            int maxChars = SCREEN_WIDTH / 6; // Approximate: 6 pixels per character
            if (maxChars > 0 && maxChars < maxLength)
            {
                textC[maxChars - 1] = '\0'; // Truncate and null-terminate
                textWidth = display.getPrintWidth(textC);
            }
        }

        // Calculate X position for centering
        int textX = (SCREEN_WIDTH - textWidth) / 2;
        display.setCursor(textX, y);
        display.print(textC);
        y += lineHeight;
    };

    // Center "Welcome!"
    centerPrint("Welcome!", cursorY);

    // Center "Press to view"
    centerPrint("Current Status:", cursorY);

    // Center "Activity Status"
    display.fontColor(TS_8b_Green, TS_8b_Black);
    centerPrint(activityStatus, cursorY);
}

void displayBatteryScreen(double temperature) {
    display.clearScreen();
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.setCursor(0, 0);
    display.print("Battery");
    display.setFont(liberationSansNarrow_10ptFontInfo);
    display.setCursor(0, 20);
    display.print("Voltage: ");
    
    float battVoltage = getSmoothedBattVoltage();
    display.print(battVoltage);
    display.print("V");
    displayBattery();
}

/**
 * @brief Updates the display based on the current screen state.
 *
 * This function checks the current screen state and updates the display accordingly.
 * It also handles user navigation between screens using the upper left and upper right buttons.
 * If the current screen is the home screen, it calls displayHomeScreen().
 * If the current screen is the fall and temperature screen, it calls displayTemperatureAndFallStatus().
 *
 * @param temperature The current temperature in Celsius.
 * @param activityStatusParam The current activity status (e.g., "RESTING", "WALKING", "RUNNING", "FALL DETECTED!").
 */
void updateDisplay(double temperature, const char *activityStatusParam)
{
    static uint8_t previousButtons = 0; // Add static variable to track previous button states
    uint8_t buttons = display.getButtons();

    // Detect newly pressed buttons
    uint8_t justPressed = buttons & (~previousButtons);

    // Handle carousel navigation
    if (justPressed & TSButtonUpperLeft)
    {
        // Navigate to previous screen
        switch (currentScreen)
        {
            case HOME_SCREEN:
                currentScreen = BATTERY_SCREEN;
                break;
            case FALL_AND_TEMP_SCREEN:
                currentScreen = HOME_SCREEN;
                break;
            case MEDICATION_INFO_SCREEN:
                currentScreen = FALL_AND_TEMP_SCREEN;
                break;
            case BATTERY_SCREEN:
                currentScreen = MEDICATION_INFO_SCREEN;
                break;
            default:
                currentScreen = HOME_SCREEN;
                break;
        }
    }
    else if (justPressed & TSButtonUpperRight)
    {
        // Navigate to next screen
        switch (currentScreen)
        {
            case HOME_SCREEN:
                currentScreen = FALL_AND_TEMP_SCREEN;
                break;
            case FALL_AND_TEMP_SCREEN:
                currentScreen = MEDICATION_INFO_SCREEN;
                break;
            case MEDICATION_INFO_SCREEN:
                currentScreen = BATTERY_SCREEN;
                break;
            case BATTERY_SCREEN:
                currentScreen = HOME_SCREEN;
                break;
            default:
                currentScreen = HOME_SCREEN;
                break;
        }
    }

    previousButtons = buttons; // Update previous button states

    // Display the current screen
    if (currentScreen == HOME_SCREEN)
    {
        displayHomeScreen();
    }
    else if (currentScreen == FALL_AND_TEMP_SCREEN)
    {
        displayTemperatureAndFallStatus(activityStatusParam, temperature);
    }
    else if (currentScreen == MEDICATION_INFO_SCREEN)
    {
        displayMedicationInfoScreen();
    }
    else if (currentScreen == MEDICATION_SCREEN)
    {
        displayMedicationScreen(); // Display popup screen
    }
    else if (currentScreen == BATTERY_SCREEN)
    {
        displayBatteryScreen(temperature);
    }
}

/**
 * @brief Updates the fall display status.
 *
 * This function checks if the fall display should continue or revert to the normal display.
 * If the fall display duration has ended, it reverts to the normal display state.
 * If the fall display is still active, it continues to display the fall status.
 * If the current display state is normal, it updates the display with the temperature and activity status.
 */
void updateFallDisplayStatus()
{
    unsigned long currentTime = millis(); // Update current time

    if (fallDetectedFlag)
    {
        if (currentTime - fallDetectedTime < 10000) // Display for 10 seconds
        {
            // Continue displaying fall message
            displayTemperatureAndFallStatus(activityStatus.c_str(), temp);
        }
        else
        {
            // Stop displaying fall message after 10 seconds
            fallDetectedFlag = false;
        }
    }
    else
    {
        // Continue with normal display updates
        updateDisplay(temp, activityStatus.c_str());
    }
}

/**
 * @brief Prints a labeled data pair with specified colors.
 *
 * @param label The label text (e.g., "Status:", "Temperature:").
 * @param value The corresponding value to display.
 * @param labelColor The color for the label.
 * @param valueColor The color for the value.
 * @param cursorX The X-coordinate for the cursor.
 * @param &cursorY The Y-coordinate for the cursor, updated after printing.
 */
void printLabeledData(const char *label, const char *value, uint8_t labelColor, uint8_t valueColor, int cursorX, int &cursorY)
{
    display.setCursor(cursorX, cursorY);
    display.fontColor(labelColor, TS_8b_Black);
    display.print(label);
    display.fontColor(valueColor, TS_8b_Black);
    display.print(value);
    cursorY += display.getFontHeight();
}

// Overloaded version for double values
void printLabeledData(const char *label, double value, uint8_t labelColor, uint8_t valueColor, int cursorX, int &cursorY, int decimalPlaces)
{
    display.setCursor(cursorX, cursorY);
    display.fontColor(labelColor, TS_8b_Black);
    display.print(label);
    display.fontColor(valueColor, TS_8b_Black);
    display.print(value, decimalPlaces);
    display.print(" C");
    cursorY += display.getFontHeight();
}

/**
 * @brief Displays the temperature and fall status on the TinyScreen.
 *
 * This function clears the screen and sets the font to display the current temperature and fall status.
 * It categorizes the temperature and displays the temperature category.
 * If a fall is detected, it sets the color based on the severity of the fall.
 *
 * @param status The current activity status (e.g., "RESTING", "WALKING", "RUNNING", "FALL DETECTED!").
 * @param temperature The current temperature in Celsius.
 */
void displayTemperatureAndFallStatus(const char *status, double temperature)
{
    display.clearScreen();

    // Set font for the header
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.fontColor(TS_8b_Yellow, TS_8b_Black);

    // Calculate the width of the header text
    int headerWidth = display.getPrintWidth("Activity Status");
    int headerX = (SCREEN_WIDTH - headerWidth) / 2; // Center the header
    display.setCursor(headerX, 0);                  // Position header at the top center
    display.print("Activity Status");

    // Set font for the details
    display.setFont(liberationSansNarrow_10ptFontInfo);
    int cursorX = 0;  // Left alignment for details
    int cursorY = 20; // Start below the header
    int lineHeight = display.getFontHeight();

    // Determine if the status indicates a fall and its severity
    bool isFall = false;
    uint8_t fallSeverity = 0; // 1: Minor, 2: Moderate, 3: Severe

    if (strcmp(status, "SEVERE FALL DETECTED!") == 0)
    {
        isFall = true;
        fallSeverity = 3;
    }
    else if (strcmp(status, "MODERATE FALL DETECTED!") == 0)
    {
        isFall = true;
        fallSeverity = 2;
    }
    else if (strcmp(status, "MINOR FALL DETECTED!") == 0)
    {
        isFall = true;
        fallSeverity = 1;
    }

    if (isFall)
    {
        // Set color based on severity
        switch (fallSeverity)
        {
        case 3: // Severe
            display.fontColor(TS_8b_Red, TS_8b_Black);
            break;
        case 2: // Moderate
            display.fontColor(TS_8b_Orange, TS_8b_Black);
            break;
        case 1: // Minor
            display.fontColor(TS_8b_Yellow, TS_8b_Black);
            break;
        default:
            display.fontColor(TS_8b_White, TS_8b_Black);
            break;
        }

        // Display Fall Status
        printLabeledData("Fall Status: ", status, TS_8b_Red, TS_8b_White, cursorX, cursorY);

        // Display Temperature
        printLabeledData("Temp: ", temperature, TS_8b_Orange, TS_8b_White, cursorX, cursorY, 1);

        // Display Temperature Category
        char category[10];
        strcpy(category, getTemperatureCategory(temperature).c_str());
        printLabeledData("Category: ", category, TS_8b_Orange, TS_8b_White, cursorX, cursorY);
    }
    else
    {
        // Display Normal Status
        printLabeledData("Status: ", status, TS_8b_Green, TS_8b_White, cursorX, cursorY);

        // Display Temperature
        printLabeledData("Temp: ", temperature, TS_8b_Orange, TS_8b_White, cursorX, cursorY, 1);

        // Display Temperature Category
        char category[10];
        strcpy(category, getTemperatureCategory(temperature).c_str());
        printLabeledData("Category: ", category, TS_8b_Orange, TS_8b_White, cursorX, cursorY);
    }
}

/**
 * @brief Categorizes the temperature.
 *
 * @param temperature The current temperature in Celsius.
 * @return String The temperature category ("Cool", "Normal", "Hot").
 */
String getTemperatureCategory(double temperature)
{
    if (temperature < 28)
    {
        return "Cool"; // COOL
    }
    else if (temperature >= 28 && temperature <= 33)
    {
        return "Normal"; // NORMAL
    }
    else
    {
        return "Hot"; // HOT
    }
}

/**
 * @brief Get the Current Time object
 *
 * This function sets the current time on the RTC module and retrieves the current time.
 *
 * @return String The current time in "HH:MM" format.
 */
String getCurrentTime()
{
    if (rtc.getHours() == NULL && rtc.getMinutes() == NULL && rtc.getSeconds() == NULL) {
        // Set the time
        rtc.setHours(hours);
        rtc.setMinutes(minutes);
        rtc.setSeconds(seconds);

        // Set the date
        rtc.setDay(day);
        rtc.setMonth(month);
        rtc.setYear(year);
    }

    // Retrieve the current time
    int currentHour = rtc.getHours();
    int currentMinute = rtc.getMinutes();

    char timeStr[6]; // "HH:MM" + null terminator
    sprintf(timeStr, "%02d:%02d", currentHour, currentMinute);
    return String(timeStr);
}

/**
 * @brief Displays the medication confirmation screen on the TinyScreen.
 *
 * This function clears the screen, sets the font and color for the header,
 * and displays the "Medication" title along with instructions for user confirmation.
 */
void displayMedicationScreen()
{
    display.clearScreen();

    // Set font and color for header
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);

    // Display "Medication" centered
    String header = currentMedication;
    int headerWidth = display.getPrintWidth(const_cast<char*>(header.c_str()));
    int headerX = (SCREEN_WIDTH - headerWidth) / 2;
    display.setCursor(headerX, 0);
    display.print(header);

    // Set font for instructions
    display.setFont(liberationSansNarrow_8ptFontInfo);
    int cursorY = display.getFontHeight() + 24; // Adjust as needed

    // Display instructions centered
    const char *instruction = "< Yes        No >";
    int instructionWidth = display.getPrintWidth(const_cast<char*>(instruction));
    int instructionX = (SCREEN_WIDTH - instructionWidth) / 2;
    display.setCursor(instructionX, cursorY);
    display.print(instruction);
}

/**
 * @brief Displays the medication information screen on the TinyScreen.
 *
 * This function clears the screen and sets the font and color for the header.
 * It displays the medication information, including medication type and time.
 */
void displayMedicationInfoScreen()
{
    display.clearScreen();

    // Set font and color for header
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);

    // Display "Medication Info" centered
    const char *header = "Medication Info";
    int headerWidth = display.getPrintWidth(const_cast<char *>(header));
    int headerX = (SCREEN_WIDTH - headerWidth) / 2;
    display.setCursor(headerX, 0);
    display.print(header);

    // Set font for medication list
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);
    int cursorY = display.getFontHeight() + 10;

    for (const auto &med : medicationSchedule)
    {
        // Format: "HH:MM, Medication"
        String medInfo = med.time + ", " + med.type;

        display.setCursor(0, cursorY);
        display.print(medInfo);
        cursorY += display.getFontHeight() + 2;
      
        // Stop if cursorY exceeds the screen height
        if (cursorY + display.getFontHeight() > SCREEN_HEIGHT)
        {
            break;
        }
    }
}

/**
 * @brief Reset the sleepTimer variable and turn on the TinyScreen if it is currently off.
 *
 * This function is currently triggered when any button is pressed. 
 */
int requestScreenOn() {
    sleepTimer = millis();
    if (!displayOn) {
        displayOn = 1;
        display.on();
        return 1;
    }
    return 0;
}

/**
 * @brief Turn off the TinyScreen after a set period of time of inactivity.
 */
void sleepDisplay() {
    if (millis() > sleepTimer + ((unsigned long)sleepTimeout * 1000ul)) {
        if (displayOn) {
            displayOn = 0;
            display.off();
        }
    }
}

/**
 * @brief Monitors the state of the buttons
 *
 * This function turns on the display if any button is pressed.
 */
void checkButtons() {
    byte buttons = display.getButtons();
    if (buttonReleased && buttons) {
        requestScreenOn();
        buttonReleased = 0;
    }
    if (!buttonReleased && !(buttons & 0x0F)) {
        buttonReleased = 1;
    }
}