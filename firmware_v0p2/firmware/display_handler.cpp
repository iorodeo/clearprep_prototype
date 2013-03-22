#include "display_handler.h"
#include "Streaming.h"

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
    setBrightness(30);
    clearScreen();
}


void DisplayHandler::showSplashScreen()
{
    lcd.print("CLEARPREP",35,15);
    lcd.print("RESOLUTION BIOMEDICAL",1,25);
    delay(constants::SplashScreenTimeout);
    lcd.clearScreen();
}


void DisplayHandler::updateBrightness()
{
    unsigned int potValue = analogRead(constants::PotentiometerPin);
    unsigned int brightness = map(potValue, 0,1023,0,100);
    lcd.setBrightness(brightness);
}

void DisplayHandler::clearScreen()
{
    lcd.clearScreen();
}

void DisplayHandler::setBrightness(unsigned int value)
{
    lcd.setBrightness(value);
}


void DisplayHandler::showValue(unsigned int value)
{
    char msg[constants::LcdNumCol];
    snprintf(msg, constants::LcdNumCol, "VALUE = %d       ", value);         
    lcd.setPos(1,10);
    lcd.print(msg);
}

void DisplayHandler::showValue(float value)
{
    char msg[constants::LcdNumCol];
    int intPart = floor(value);
    int decPart = int(100*(value - float(intPart)));
    snprintf(msg, constants::LcdNumCol, "ABSORBANCE: %d.%d          ", intPart, decPart);
    lcd.setPos(15,10);
    lcd.print(msg);
}


void DisplayHandler::showTriggered()
{
    char msg[constants::LcdNumCol];
    snprintf(msg, constants::LcdNumCol, "Triggered");
    lcd.clearScreen();
    lcd.setPos(1,10);
    lcd.print(msg);
}




