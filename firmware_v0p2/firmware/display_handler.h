#ifndef DISPLAY_HANDLER_HPP
#define DISPLAY_HANDLER_HPP
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <SoftwareSerial.h>
#include "SerialLCD.h"
#include "constants.h"

class DisplayHandler
{

    public:

        DisplayHandler();
        void initialize();
        void showSplashScreen();
        void setBrightness(unsigned int value);
        void clearScreen();

        void updateMeasurementScreen(float value, float threshUpper, float threshLower);
        void updateShowThresholdScreen(float threshUpper, unsigned int okSetValue);
        void updateSetThresholdScreen(float value, float threshUpper);

    private:
        static SoftwareSerial softwareSerial;
        static SerialLCD lcd;

};


#endif // #ifndef DISPLAY_HANDLER_HPP



