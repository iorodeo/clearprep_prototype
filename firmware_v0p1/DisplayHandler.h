#ifndef _DISPLAY_HANDLER_H_
#define _DISPLAY_HANDLER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SerialLCD.h"
#include "constants.h"


class DisplayHandler {
    private:
        SerialLCD _lcd;
        uint8_t _tracePlotPos;
        uint8_t _traceClearPos;
        uint16_t _traceBuf[DISPLAY_TRACE_BUF_LEN];
        float _traceAbsorbMin;
        float _traceAbsorbMax;
    public:
        //DisplayHandler(Print &port);
        DisplayHandler(Stream &port);
        void initialize();
        void setBrightness();
        void showSplashScreen();
        void updateAbsorbText(float absorb);
        void updateTracePlot(float absorb);
        void setTraceMin(float absorbMin);
        void setTraceMax(float absorbMax);
};

#endif
