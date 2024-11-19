#include "../include/display.h"
#include "../include/battery.h"

// This function gets the battery VCC internally, you can checkout this link 
// if you want to know more about how: 
// http://atmel.force.com/support/articles/en_US/FAQ/ADC-example
float getVCC() {
    SYSCTRL->VREF.reg |= SYSCTRL_VREF_BGOUTEN;
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->SAMPCTRL.bit.SAMPLEN = 0x1;
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->INPUTCTRL.bit.MUXPOS = 0x19;         // Internal bandgap input
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->CTRLA.bit.ENABLE = 0x01;             // Enable ADC
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->SWTRIG.bit.START = 1;  // Start conversion
    ADC->INTFLAG.bit.RESRDY = 1;  // Clear the Data Ready flag
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->SWTRIG.bit.START = 1;  // Start the conversion again to throw out first value
    while ( ADC->INTFLAG.bit.RESRDY == 0 );   // Waiting for conversion to complete
    uint32_t valueRead = ADC->RESULT.reg;
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->CTRLA.bit.ENABLE = 0x00;             // Disable ADC
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    SYSCTRL->VREF.reg &= ~SYSCTRL_VREF_BGOUTEN;
    float vcc = (1.1 * 1023.0) / valueRead;
    return vcc;
}

// Calculate the battery voltage
float getBattVoltage() {
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

void displayBattery() {
    uint8_t x = 70;
    uint8_t y = 3;
    uint8_t height = 5;
    uint8_t length = 20;
    uint8_t red, green;
    
    float battVoltage = getBattVoltage();

    display.drawLine(x - 1, y, x - 1, y + height, 0xFF); //left boarder
    display.drawLine(x - 1, y - 1, x + length, y - 1, 0xFF); //top border
    display.drawLine(x - 1, y + height + 1, x + length, y + height + 1, 0xFF); //bottom border
    display.drawLine(x + length, y - 1, x + length, y + height + 1, 0xFF); //right border
    display.drawLine(x + length + 1, y + 2, x + length + 1, y + height - 2, 0xFF); //right border

    // 75% - 100%
    if (battVoltage >= 3.60) {
        red = 0;
        green = 63;
        for (uint8_t i = 0; i < length; i++) {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 50% - 75%
    else if (battVoltage >= 3.42) {
        red = 30;
        green = 63;
        for (uint8_t i = 0; i < length*0.75; i++) {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 25% - 50%
    else if (battVoltage >= 3.32) {
        red = 63;
        green = 63;
        for (uint8_t i = 0; i < length*0.5; i++) {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 0% - 25%
    else if (battVoltage >= 3.00) {
        red = 63;
        green = 0;
        for (uint8_t i = 0; i < length*0.25; i++) {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
    // 0% (battery going to die soon)
    else {
        red = 63;
        green = 0;
        for (uint8_t i = 0; i < 1; i++) {
            display.drawLine(x + i, y, x + i, y + height, red, green, 0);
        }
    }
}