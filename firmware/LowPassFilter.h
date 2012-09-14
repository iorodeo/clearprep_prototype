#ifndef _LOW_PASS_H_
#define _LOW_PASS_H_

class LowPassFilter {
    public:
        LowPassFilter();
        LowPassFilter(float freqCut);
        LowPassFilter(float freqCut, float initValue);
        void setValue(float value);
        float getValue();
        void setFreqCut(float freq); 
        float getFreqCut();
        float update(float rawValue, float dt);
    private:
        float _value;
        float _freqCut;
        float _rc;
};



#endif
