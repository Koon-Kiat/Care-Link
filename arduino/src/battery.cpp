#include "../include/display.h"
#include "../include/battery.h"


double alpha = 0.001; // Smoothing factor (0 < ALPHA <= 1)

bool lowBatteryAlertShown = false; // Flag to prevent multiple low battery alerts

//String batteryStatus = "";

// This function gets the battery VCC internally, you can checkout this link
// if you want to know more about how:
// http://atmel.force.com/support/articles/en_US/FAQ/ADC-example
float getVCC()
{
    SYSCTRL->VREF.reg |= SYSCTRL_VREF_BGOUTEN;
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    ADC->SAMPCTRL.bit.SAMPLEN = 0x1;
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    ADC->INPUTCTRL.bit.MUXPOS = 0x19; // Internal bandgap input
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    ADC->CTRLA.bit.ENABLE = 0x01; // Enable ADC
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    ADC->SWTRIG.bit.START = 1;   // Start conversion
    ADC->INTFLAG.bit.RESRDY = 1; // Clear the Data Ready flag
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    ADC->SWTRIG.bit.START = 1; // Start the conversion again to throw out first value
    while (ADC->INTFLAG.bit.RESRDY == 0)
        ; // Waiting for conversion to complete
    uint32_t valueRead = ADC->RESULT.reg;
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    ADC->CTRLA.bit.ENABLE = 0x00; // Disable ADC
    while (ADC->STATUS.bit.SYNCBUSY == 1)
        ;
    SYSCTRL->VREF.reg &= ~SYSCTRL_VREF_BGOUTEN;
    float vcc = (1.1 * 1023.0) / valueRead;
    return vcc;
}

// Calculate the battery voltage
float getBattVoltage()
{
    const int VBATTpin = A4;
    float VCC = getVCC();

    // Use resistor division and math to get the voltage
    float resistorDiv = 0.5;
    float ADCres = 1023.0;
    float battVoltageReading = analogRead(VBATTpin);
    battVoltageReading = analogRead(VBATTpin); // Throw out first value
    float battVoltage = VCC * battVoltageReading / ADCres / resistorDiv;

    return battVoltage;
}

void displayBattery()
{
    uint8_t x = 70;
    uint8_t y = 3;
    uint8_t height = 5;
    uint8_t length = 20;
    uint8_t red, green;

    float battVoltage = getSmoothedBattVoltage();

    display.drawLine(x - 1, y, x - 1, y + height, 0xFF);                           // Left border
    display.drawLine(x - 1, y - 1, x + length, y - 1, 0xFF);                       // Top border
    display.drawLine(x - 1, y + height + 1, x + length, y + height + 1, 0xFF);     // Bottom border
    display.drawLine(x + length, y - 1, x + length, y + height + 1, 0xFF);         // Right border
    display.drawLine(x + length + 1, y + 2, x + length + 1, y + height - 2, 0xFF); // Right border

    // Close to 100%
    if (battVoltage >= 3.90)
    {
        red = 0;
        green = 63;
        batteryStatus = "FULLY CHARGED";
        for (uint8_t i = 0; i < length; i++)
        {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 75% - 100%
    if (battVoltage >= 3.60)
    {
        red = 0;
        green = 63;
        batteryStatus = "CHARGED";
        for (uint8_t i = 0; i < length; i++)
        {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 50% - 75%
    else if (battVoltage >= 3.42)
    {
        red = 30;
        green = 63;
        batteryStatus = "NORMAL";
        for (uint8_t i = 0; i < length * 0.75; i++)
        {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 25% - 50%
    else if (battVoltage >= 3.32)
    {
        red = 63;
        green = 63;
        batteryStatus = "NORMAL";
        for (uint8_t i = 0; i < length * 0.5; i++)
        {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 0% - 25%
    else if (battVoltage >= 3.00)
    {
        red = 63;
        green = 0;
        batteryStatus = "LOW";
        for (uint8_t i = 0; i < length * 0.25; i++)
        {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 0% (battery going to die soon)
    else
    {
        red = 63;
        green = 0;
        batteryStatus = "EMPTY";
        for (uint8_t i = 0; i < 1; i++)
        {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
}

void lowBatteryAlert()
{

    float battVoltage = getSmoothedBattVoltage();

    // Once the set voltage has been reached, display the low battery alert once.
    if (!(battVoltage >= 3.32) && !(lowBatteryAlertShown))
    {
        display.clearScreen();
        display.setFont(liberationSansNarrow_12ptFontInfo);
        display.fontColor(TS_8b_Red, TS_8b_Black);
        display.setCursor(0, 0);
        display.print("Low Battery");
        display.setFont(liberationSansNarrow_10ptFontInfo);
        display.setCursor(0, 20);
        display.print("Please charge");
        display.setCursor(0, 35);
        display.print("the watch.");
        delay(5000);
        lowBatteryAlertShown = true;
    }
}

float getSmoothedBattVoltage()
{
    static float ema = 0;
    static bool initialized = false;

    float newReading = getBattVoltage();

    if (!initialized)
    {
        ema = newReading;
        initialized = true;
    }
    else
    {
        ema = (alpha * newReading) + ((1 - alpha) * ema);
    }

    return ema;
}