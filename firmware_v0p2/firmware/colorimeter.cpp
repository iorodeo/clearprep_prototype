#include <math.h>
#include "colorimeter.h"
#include "constants.h"
#include "eeprom_anything.h"
#include "Streaming.h"

const uint8_t EEPROM_CAL_RED = 0; 
const uint8_t EEPROM_CAL_GREEN = 4; 
const uint8_t EEPROM_CAL_BLUE = 8;
const uint8_t EEPROM_CAL_WHITE = 12;

Colorimeter::Colorimeter() {
    numSamples = DEFAULT_NUM_SAMPLES;
    calibration.red = 1;
    calibration.green = 1; 
    calibration.blue = 1;
    calibration.white = 1;
}


void Colorimeter::initialize( 
        uint8_t redLedPin, 
        uint8_t greenLedPin, 
        uint8_t blueLedPin,
        uint8_t sensorS0, 
        uint8_t sensorS1, 
        uint8_t sensorS2, 
        uint8_t sensorS3, 
        uint8_t sensorFO
        )
{
    led.initialize(redLedPin, greenLedPin, blueLedPin);
    sensor.initialize(sensorS0, sensorS1, sensorS2, sensorS3, sensorFO);
}


// Sample frequency methods 
// ----------------------------------------------------------------------------
uint32_t Colorimeter::getFrequencyRed() {
    led.setRed();
    sensor.setChannelRed();
    return sensor.getFrequency(numSamples);
}

uint32_t Colorimeter::getFrequencyGreen() {
    led.setGreen();
    sensor.setChannelGreen();
    return sensor.getFrequency(numSamples);
}

uint32_t Colorimeter::getFrequencyBlue() {
    led.setBlue();
    sensor.setChannelBlue();
    return sensor.getFrequency(numSamples);
}

uint32_t Colorimeter::getFrequencyWhite() {
    led.setWhite();
    sensor.setChannelClear();
    return sensor.getFrequency(numSamples);
}

float Colorimeter::getAbsorbanceRed() {
    float abso;
    float tran;
    uint32_t freq;

    freq = getFrequencyRed();
    tran = freq2trans(calibration.red,freq);
    abso = trans2absorb(tran);
    //Serial << "freq:" << freq << endl;
    return abso;
}

void Colorimeter::getMeasurement() {
    frequency.red = getFrequencyRed();
    frequency.green = getFrequencyGreen();
    frequency.blue = getFrequencyBlue();
    frequency.white = getFrequencyWhite();
    led.setOff();

    transmission.red = freq2trans(calibration.red, frequency.red);
    transmission.green = freq2trans(calibration.green, frequency.green);
    transmission.blue = freq2trans(calibration.blue, frequency.blue);
    transmission.white = freq2trans(calibration.white, frequency.white);

    absorbance.red = trans2absorb(transmission.red);
    absorbance.green = trans2absorb(transmission.green);
    absorbance.blue = trans2absorb(transmission.blue); 
    absorbance.white = trans2absorb(transmission.white);
}

// Calibration methods // ----------------------------------------------------------------------------
void Colorimeter::calibrate() {
    calibration.red = getFrequencyRed();
    calibration.green = getFrequencyGreen();
    calibration.blue = getFrequencyBlue();
    calibration.white = getFrequencyWhite();
    led.setOff();
}

void Colorimeter::EEPROM_saveCalibration() {
    EEPROM_writeAnything(EEPROM_CAL_RED,calibration.red);
    EEPROM_writeAnything(EEPROM_CAL_GREEN,calibration.green);
    EEPROM_writeAnything(EEPROM_CAL_BLUE,calibration.blue);
    EEPROM_writeAnything(EEPROM_CAL_WHITE,calibration.white);
}

void Colorimeter::EEPROM_loadCalibration() {
    EEPROM_readAnything(EEPROM_CAL_RED,calibration.red);
    EEPROM_readAnything(EEPROM_CAL_GREEN,calibration.green);
    EEPROM_readAnything(EEPROM_CAL_BLUE,calibration.blue);
    EEPROM_readAnything(EEPROM_CAL_WHITE,calibration.white);
}

bool Colorimeter::checkCalibration() {
    bool flag = true;
    if (calibration.red == 0) {
        flag = false;
    }
    if (calibration.green == 0) {
        flag = false;
    }
    if (calibration.blue == 0) {
        flag = false;
    }
    if (calibration.white == 0) {
        flag = false;
    }
    return flag;
}


// Utility functions
// ----------------------------------------------------------------------------
float freq2trans(uint32_t calFreq, uint32_t sampleFreq) {
    float trans;
    if (calFreq == 0) {
        trans = -1.0;
    }
    else {
        trans = ((float) sampleFreq)/((float) calFreq);
        if (trans > 1.0) {
            trans = 1.0;
        }
    }
    return trans;
}

float trans2absorb(float transmission) { 
    float absorb;
    if (transmission > 0) {
        absorb = (float)log10( 1.0/((double) transmission));
    }
    else {
        absorb = -1.0;
    }
    return absorb;
}

