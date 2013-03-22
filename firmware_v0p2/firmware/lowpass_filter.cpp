#include <math.h>
#include "lowpass_filter.h"

LowPassFilter::LowPassFilter() {
    setFreqCut(1.0);
    setValue(0.0);
}

LowPassFilter::LowPassFilter(float freqCut) {
    setFreqCut(freqCut);
    setValue(0.0);
}

LowPassFilter::LowPassFilter(float freqCut, float initValue) {
    setFreqCut(freqCut);
    setValue(initValue);
}

void LowPassFilter::setValue(float value) {
    _value = value;
}

float LowPassFilter::getValue() {
    return _value;
}

void LowPassFilter::setFreqCut(float freqCut) {
    _freqCut = freqCut;
    _rc = 0.5/(M_PI*freqCut);
}

float LowPassFilter::getFreqCut() {
    return _freqCut;
}

float LowPassFilter::update(float rawValue, float dt) {
    float alpha;
    alpha = dt/(_rc + dt);
    _value = alpha*rawValue + (1-alpha)*_value;
    return _value;
} 
