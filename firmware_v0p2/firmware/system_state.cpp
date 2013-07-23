#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "system_state.h"
#include "Streaming.h"
#include "eeprom_anything.h"

DisplayHandler SystemState::display = DisplayHandler();


float getUpdateDt(uint32_t t1, uint32_t t0);


SystemState::SystemState() 
{ 
    isFirstSample_ = true;
    absorbRaw_ = 0.0;
    absorbFilt_ = 0.0;
    lastUpdateTime_ = 0;
    lowPass_.setFreqCut(constants::LowPassFreqCut);

    mode_ = MeasurementMode;
    modeSwitchCount_ = 0;

    absorbThreshLower_ = 0.1;
    absorbThreshUpper_ = 1.2;
}


void SystemState::initialize()
{
    setupDioPins();
    colorimeter_.initialize( 
            constants::MeasurementRedLedPin,
            constants::MeasurementGreenLedPin,
            constants::MeasurementBlueLedPin,
            constants::ColorSensorS0, 
            constants::ColorSensorS1, 
            constants::ColorSensorS2, 
            constants::ColorSensorS3, 
            constants::ColorSensorFO
            );
    colorimeter_.numSamples = constants::NumberOfSamples;
    colorimeter_.calibration.red = constants::RedLedCalibration;

    Serial.begin(constants::UsbBaudrate);
    display.initialize();
    display.showSplashScreen();
    readThresholdFromEEPROM();
}


void SystemState::update()
{
    updateMeasurement();
    updatePotValue();
    switch (mode_)
    {
        case MeasurementMode:
            updateLCDMeasMode();
            break;

        case ShowThresholdMode:
            updateLCDShowThreshMode();
            break;

        case SetThresholdMode:
            updateAbsorbThreshUpper();
            updateLCDSetThreshMode();
            break;

        default:
            updateLCDMeasMode();
            break;
    }
    updateLEDDisplay();
    updateMode();
}


void SystemState::updateMeasurement()
{
    unsigned long currentTime = micros(); 
    absorbRaw_ = colorimeter_.getAbsorbanceRed();

    if (isFirstSample_)
    {
        lowPass_.setValue(absorbRaw_);
        absorbFilt_ = absorbRaw_;
        isFirstSample_= false;
        lastUpdateTime_ = currentTime;
    }
    else
    {
        float dt = getUpdateDt(currentTime, lastUpdateTime_);
        lastUpdateTime_ = currentTime;
        absorbFilt_ = lowPass_.update(absorbRaw_,dt);
    }
}


void SystemState::updatePotValue()
{
    potValue_ = analogRead(constants::PotentiometerPin);
}


void SystemState::updateLEDDisplay()
{
    if (absorbFilt_ > absorbThreshUpper_)
    {
        digitalWrite(constants::IndicatorRedLedPin, HIGH);
        digitalWrite(constants::IndicatorGreenLedPin, LOW);
    }
    else if (absorbFilt_ > absorbThreshLower_)
    {
        digitalWrite(constants::IndicatorRedLedPin, LOW);
        digitalWrite(constants::IndicatorGreenLedPin, HIGH);
    }
    else
    {
        digitalWrite(constants::IndicatorRedLedPin, LOW);
        digitalWrite(constants::IndicatorGreenLedPin, LOW);
    }
}


void SystemState::updateLCDMeasMode()
{ 
    display.setBrightness(potValue_);
    display.updateMeasurementScreen(
            absorbFilt_,
            absorbThreshLower_, 
            absorbThreshUpper_
            );
}


void SystemState::updateLCDShowThreshMode()
{ 
    unsigned int okSetValue = getOkSetValue();
    display.updateShowThresholdScreen(absorbThreshUpper_,okSetValue);
}


void SystemState::updateLCDSetThreshMode()
{
    display.updateSetThresholdScreen(absorbFilt_, absorbThreshUpper_);
}


Mode SystemState::getMode()
{
    return mode_;
}


void SystemState::setMode(Mode newMode)
{
    mode_ = newMode;
}


void SystemState::updateMode()
{
    updateModeSwitchCount();
    if (modeSwitchCount_ >= constants::ModeSwitchCountThreshold )
    {
        modeSwitchCount_ = 0;

        switch (mode_)
        {
            case MeasurementMode:
                //mode_ = ShowThresholdMode;
                mode_ = SetThresholdMode;
                display.clearScreen();
                break;

            //case ShowThresholdMode:
            //    if (potValue_ > constants::MaxPotentiometerValue/2)
            //    {
            //        mode_ = MeasurementMode;
            //    }
            //    else
            //    {
            //        mode_ = SetThresholdMode;
            //    }
            //    display.clearScreen();
            //    break;

            case SetThresholdMode:
                
                saveThresholdToEEPROM();
                //mode_ = ShowThresholdMode;
                mode_ = MeasurementMode;
                display.clearScreen();
                break;

            default:
                mode_ = MeasurementMode;
                display.clearScreen();
                break;
        }
    }
}


void SystemState::updateModeSwitchCount()
{
    int buttonValue = digitalRead(constants::PushButtonPin);
    if (buttonValue == 0)
    {
        modeSwitchCount_++;
    }
    else
    {
        modeSwitchCount_ = 0;
    }
    Serial << buttonValue << ", " << modeSwitchCount_ << endl;
}


void SystemState::updateAbsorbThreshUpper()
{
    unsigned int absorbThreshInt = map(
            potValue_, 
            constants::MinPotentiometerValue,
            constants::MaxPotentiometerValue,
            1000*constants::MinAbsorbThresholdUpper,
            1000*constants::MaxAbsorbThresholdUpper
            );
    absorbThreshUpper_ = 0.001*float(absorbThreshInt);
}


void SystemState::setupDioPins()
{
    // Setup push button pins
    pinMode(constants::PotentiometerPin, INPUT);
    pinMode(constants::PushButtonPin, INPUT);
    digitalWrite(constants::PushButtonPin, HIGH);

    // Setup indicator led pins
    pinMode(constants::IndicatorRedLedPin, OUTPUT);
    pinMode(constants::IndicatorGreenLedPin, OUTPUT);

    digitalWrite(constants::IndicatorRedLedPin, LOW);
    digitalWrite(constants::IndicatorGreenLedPin, LOW);
}


unsigned int SystemState::getOkSetValue()
{
    if (potValue_ > constants::MaxPotentiometerValue/2)
    {
        return constants::Ok;
    }
    else
    {
        return constants::Set;
    }
}


void SystemState::readThresholdFromEEPROM()
{
    EEPROM_readAnything(0,absorbThreshUpper_);
}


void SystemState::saveThresholdToEEPROM()
{
    EEPROM_writeAnything(0,absorbThreshUpper_);
}


float getUpdateDt(uint32_t t1, uint32_t t0)  {
    float dt;
    if (t1 > t0) {
        dt = t1 - t0;
    }
    else {
        dt = t1 + (UINT32_MAX - t0);
    }
    dt = 1.0e-6*dt;
    return dt;
}
