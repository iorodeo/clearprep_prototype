#include "constants.h"

namespace constants
{
    // Pin definitions
    const int LcdSerialRxPin = 12;
    const int LcdSerialTxPin = 13;
    const int IndicatorRedLedPin = 3;
    const int IndicatorGreenLedPin = A0;
    const int PotentiometerPin = A1;
    const int PushButtonPin = 2;
   
    const int MeasurementBlueLedPin = 9; 
    const int MeasurementGreenLedPin = 10; 
    const int MeasurementRedLedPin = 11;

    const int ColorSensorS0 = 4; 
    const int ColorSensorS1 = 5; 
    const int ColorSensorS2 = 6; 
    const int ColorSensorS3 = 7; 
    const int ColorSensorFO = 8;       

    // Serial communication parameters 
    const unsigned long UsbBaudrate = 9600;
    const unsigned long LcdBaudrate = 115200;
    const unsigned int LcdStartupDelay = 1500;
    
    // Display
    const unsigned int ModeSwitchCountThreshold = 10;
    const unsigned int SplashScreenTimeout = 3000; 
    const unsigned int MinPotentiometerValue = 0; 
    const unsigned int MaxPotentiometerValue = 1023; 
    const unsigned int MinBrightnessValue = 0;
    const unsigned int MaxBrightnessValue = 100;
    const unsigned int ModeChangeDelay = 500;

    // Signal processing
    const float LowPassFreqCut = 1.5;
    extern const float MinAbsorbThresholdUpper = 0.206;
    extern const float MaxAbsorbThresholdUpper = 2.5;
    const unsigned long RedLedCalibration = 25000;
    const unsigned long GreenLedCalibration = 25000;
    const unsigned long BlueLedCalibration = 25000;
    const unsigned int NumberOfSamples = 2;

    // Set aborbance threshold parameters
    const float AbsorbThreshMinReqChange = 0.1;

    // EEPROM
    const int aborbThreshAddress=0;

} // namespace constants

