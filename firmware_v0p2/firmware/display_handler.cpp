#include "display_handler.h"
#include "Streaming.h"

int getIntegerPart(float value);
unsigned int getDecimalPart(float value);

SoftwareSerial DisplayHandler::softwareSerial = SoftwareSerial(
        constants::LcdSerialRxPin, 
        constants::LcdSerialTxPin
        );


SerialLCD DisplayHandler::lcd = SerialLCD(softwareSerial);


DisplayHandler::DisplayHandler() { }


void DisplayHandler::initialize()
{ 
    softwareSerial.begin(constants::LcdBaudrate);
    delay(constants::LcdStartupDelay);
    setBrightness(500);
    clearScreen();
}


void DisplayHandler::showSplashScreen()
{
    lcd.print("CLEARPREP",35,15);
    lcd.print("RESOLUTION BIOMEDICAL",1,25);
    delay(constants::SplashScreenTimeout);
    lcd.clearScreen();
}


void DisplayHandler::setBrightness(unsigned int potValue)
{
    unsigned int brightness = map(
            potValue, 
            constants::MinPotentiometerValue,
            constants::MaxPotentiometerValue,
            constants::MinBrightnessValue,
            constants::MaxBrightnessValue
            );
    lcd.setBrightness(brightness);
}

void DisplayHandler::clearScreen()
{
    lcd.clearScreen();
}


void DisplayHandler::updateMeasurementScreen(float value, float threshLower, float threshUpper )
{
    char msg[constants::LcdNumCol];

    // Display Threshold
    // -----------------
    int x = 20;
    int y = 10;
    int intPart = floor(threshUpper);
    int decPart = int(100*(threshUpper - float(intPart)));
    snprintf(msg, constants::LcdNumCol, "THRESHOLD:  %d.%02d         ", intPart, decPart);
    lcd.setPos(x,y);
    lcd.print(msg);

    // Display Absorbance
    // ------------------
    x = 20;
    y = 22;
    intPart = floor(value);
    decPart = int(100*(value - float(intPart)));
    snprintf(msg, constants::LcdNumCol, "ABSORBANCE: %d.%02d          ", intPart, decPart);
    lcd.setPos(x,y);
    lcd.print(msg);

    // Display Pass/Fail
    // -----------------
    x = 55;
    y = 44;
    if (value > threshUpper)
    {
        snprintf(msg, constants::LcdNumCol, "FAIL            ");
        lcd.setPos(x,y);
        lcd.print(msg);
    }
    else if (value > threshLower)
    {
        snprintf(msg, constants::LcdNumCol, "PASS            ");
        lcd.setPos(x,y);
        lcd.print(msg);
    }
    else
    {
        snprintf(msg, constants::LcdNumCol, "                ");
        lcd.setPos(x,y);
        lcd.print(msg);
    }
}

void DisplayHandler::updateShowThresholdScreen(float threshUpper, unsigned int okSetValue)
{
    char msg[constants::LcdNumCol];
    int x = 20;
    int y = 10;
    int intPart = floor(threshUpper);
    int decPart = int(100*(threshUpper - float(intPart)));
    snprintf(msg, constants::LcdNumCol, "THRESHOLD: %d.%02d          ", intPart, decPart);
    lcd.setPos(x,y);
    lcd.print(msg);

    y = 25;
    char marker = (okSetValue == constants::Ok) ? '*' : ' ';
    snprintf(msg, constants::LcdNumCol, "%c OK            ",marker);
    lcd.setPos(x,y);
    lcd.print(msg);

    y = 35;
    marker = (okSetValue == constants::Set) ? '*' : ' ';
    snprintf(msg, constants::LcdNumCol, "%c SET            ",marker);
    lcd.setPos(x,y);
    lcd.print(msg);

}


void DisplayHandler::updateSetThresholdScreen(float value, float threshUpper)
{
    char msg[constants::LcdNumCol];

    int x = 25;
    int y = 10;
    snprintf(msg, constants::LcdNumCol, "SET THRESHOLD");
    lcd.setPos(x,y);
    lcd.print(msg);

    x = 16;
    y = 17;
    snprintf(msg, constants::LcdNumCol, "----------------");
    lcd.setPos(x,y);
    lcd.print(msg);

    x = 20;
    y = 32;
    int intPart = floor(threshUpper);
    int decPart = int(100*(threshUpper - float(intPart)));
    snprintf(msg, constants::LcdNumCol, "THRESHOLD:  %d.%02d          ", intPart, decPart);
    lcd.setPos(x,y);
    lcd.print(msg);

    y = 44;
    intPart = floor(value);
    decPart = int(100*(value - float(intPart)));
    snprintf(msg, constants::LcdNumCol, "ABSORBANCE: %d.%02d          ", intPart, decPart);
    lcd.setPos(x,y);
    lcd.print(msg);

}

