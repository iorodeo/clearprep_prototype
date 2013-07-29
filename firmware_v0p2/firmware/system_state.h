#ifndef SYSTEM_STATE_HPP
#define SYSTEM_STATE_HPP
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "constants.h"
#include "display_handler.h"
#include "lowpass_filter.h"
#include "colorimeter.h"

enum Mode {
    MeasurementMode=0, 
    ShowThresholdMode,
    SetThresholdMode,
}; 

class SystemState
{

    public:
        static DisplayHandler display;

        SystemState();
        void initialize();
        void update();

    private:

        Colorimeter colorimeter_;
        LowPassFilter lowPass_;

        bool isFirstSample_;
        bool absorbThreshUpperChanged_;
        bool isAbsorbThreshUpperFirstValue_;;
        unsigned long lastUpdateTime_;

        float absorbRaw_;
        float absorbFilt_;
        float absorbThreshUpper_;
        float absorbThreshLower_;

        Mode mode_;
        unsigned int modeSwitchCount_;
        unsigned int potValue_;

        void setupDioPins();
        void updateMode();
        void updatePotValue();
        void updateMeasurement();
        void updateModeSwitchCount();
        void updateAbsorbThreshUpper();

        void updateLEDDisplay();
        void updateLCDMeasMode();
        void updateLCDShowThreshMode();
        void updateLCDSetThreshMode();

        Mode getMode();
        void setMode(Mode newMode);

        unsigned int getOkSetValue();
        
        void readThresholdFromEEPROM();
        void saveThresholdToEEPROM();


};

#endif // SYSTEM_STATE_HPP
