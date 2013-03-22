#ifndef _COLORIMETER_H_
#define _COLORIMETER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "color_sensor.h"
#include "rgb_led.h"

const uint16_t DEFAULT_NUM_SAMPLES = 500;

template <class T>
class ColorimeterData {
    public:
        T red;
        T green;
        T blue;
        T white;
};
            
class Colorimeter {
    public:

        Colorimeter();
        void initialize(
                uint8_t redLedPin, 
                uint8_t greenLedPin, 
                uint8_t blueLedPin,
                uint8_t sensorS0, 
                uint8_t sensorS1, 
                uint8_t sensorS2, 
                uint8_t sensorS3, 
                uint8_t sensorFO
                );

        RGBLed led;
        ColorSensor sensor;

        uint32_t getFrequencyRed();
        uint32_t getFrequencyGreen();
        uint32_t getFrequencyBlue();
        uint32_t getFrequencyWhite();
        void getMeasurement();

        float getAbsorbanceRed();

        void calibrate();
        bool checkCalibration();
        void EEPROM_saveCalibration();
        void EEPROM_loadCalibration();

        uint16_t numSamples;
        ColorimeterData<uint32_t> calibration;
        ColorimeterData<uint32_t> frequency; 
        ColorimeterData<float> transmission;
        ColorimeterData<float> absorbance;
};

float freq2trans(uint32_t calFreq, uint32_t sampleFreq);

float trans2absorb(float transmission);
#endif
