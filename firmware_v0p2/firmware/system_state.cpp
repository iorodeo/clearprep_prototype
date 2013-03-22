#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "system_state.h"
#include "Streaming.h"

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

    // ------------------------------------------------------------------------
    // TEMPORARY
    // ------------------------------------------------------------------------
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

}


void SystemState::update()
{
    updateMeasurement();
    updateDisplay();
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


void SystemState::updateDisplay()
{
    display.updateBrightness();

    // ------------------------------------------------------------------------
    // TEMPORARY 
    // ------------------------------------------------------------------------
    if (mode_ == MeasurementMode)
    {
        display.showValue(absorbFilt_);
        // Set indicator led
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
    else
    {
        display.showTriggered();
    }
    // ------------------------------------------------------------------------

    
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
                mode_ = ThresholdMode;
                display.clearScreen();
                break;

            case ThresholdMode:
                mode_ = MeasurementMode;
                display.clearScreen();
                break;

            default:
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
