#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace constants
{
    // Pin definitions
    extern const int LcdSerialRxPin;
    extern const int LcdSerialTxPin;

    extern const int IndicatorRedLedPin;
    extern const int IndicatorGreenLedPin;
    
    extern const int PotentiometerPin;
    extern const int PushButtonPin;

    extern const int MeasurementBlueLedPin; 
    extern const int MeasurementGreenLedPin; 
    extern const int MeasurementRedLedPin;

    extern const int ColorSensorS0; 
    extern const int ColorSensorS1; 
    extern const int ColorSensorS2; 
    extern const int ColorSensorS3; 
    extern const int ColorSensorFO;       

    // Serial communication parameters 
    extern const unsigned long UsbBaudrate;
    extern const unsigned long LcdBaudrate;
    extern const unsigned int LcdStartupDelay;

    // Display
    enum {LcdNumRow=64, LcdNumCol=128};
    enum {Ok=0, Set};
    extern const unsigned int ModeSwitchCountThreshold;
    extern const unsigned int SplashScreenTimeout;
    extern const unsigned int MinPotentiometerValue; 
    extern const unsigned int MaxPotentiometerValue; 
    extern const unsigned int MinBrightnessValue;
    extern const unsigned int MaxBrightnessValue;
    extern const unsigned int ModeChangeDelay;
    
    // Signal processing
    extern const float LowPassFreqCut;
    extern const float MinAbsorbThresholdUpper;
    extern const float MaxAbsorbThresholdUpper;
    extern const unsigned long RedLedCalibration;
    extern const unsigned long GreenLedCalibration;
    extern const unsigned long BlueLedCalibration;
    extern const unsigned int NumberOfSamples;

    // Set Threshold parameters
    extern const float AbsorbThreshMinReqChange;

} // namespace constants

#endif // #ifndef CONSTANTS_HPP
