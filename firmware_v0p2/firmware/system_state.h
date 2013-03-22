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

enum Mode {MeasurementMode=0, ThresholdMode=1}; 

class SystemState
{

    public:
        static DisplayHandler display;

        SystemState();
        void initialize();

        void update();
        void updateMode();
        void updateDisplay();
        void updateMeasurement();
        void updateModeSwitchCount();

        Mode getMode();
        void setMode(Mode newMode);

    private:

        Colorimeter colorimeter_;
        LowPassFilter lowPass_;

        bool isFirstSample_;
        unsigned long lastUpdateTime_;

        float absorbRaw_;
        float absorbFilt_;
        float absorbThreshUpper_;
        float absorbThreshLower_;

        Mode mode_;
        unsigned int modeSwitchCount_;

        void setupDioPins();
};

#endif // SYSTEM_STATE_HPP
