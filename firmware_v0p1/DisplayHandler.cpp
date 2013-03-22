#include "util/atomic.h"
#include "DisplayHandler.h"


DisplayHandler::DisplayHandler(Stream &port) {
    _lcd.setPort(port); 
    for (int i=1; i<DISPLAY_TRACE_BUF_LEN; i++) {
        _traceBuf[i] = 0;
    }
    _tracePlotPos = 0;
    _traceClearPos = DISPLAY_CLEAR_OFFSET;
}

void DisplayHandler::initialize() {
    _lcd.clearScreen();
    setBrightness();
    setTraceMin(DISPLAY_TRACE_MIN);
    setTraceMax(DISPLAY_TRACE_MAX);
}

void DisplayHandler::setTraceMin(float absorbMin) {
    _traceAbsorbMin = absorbMin;
}

void DisplayHandler::setTraceMax(float absorbMax) {
    _traceAbsorbMax = absorbMax;
}

void DisplayHandler::setBrightness() {
    // Read Potentiometer value and use value to set brightness
    // of LCD.
    int pot;
    int brightness;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        pot = analogRead(DISPLAY_POT_AIN);
    }
    brightness = map(pot,1024,0,0,100);
    _lcd.setBrightness(brightness);
}

void DisplayHandler::showSplashScreen() {
    // Draw Splash Screen w/ IO Rodeo logo.
    //_lcd.drawBox(1,1,127,63,1);
    _lcd.print("CLEARPREP",35,10);
    _lcd.print("RESOLUTION BIOMEDICAL",1,30);
    //_lcd.print("BIOMEDICAL",65,52);
    delay(DISPLAY_SPLASH_TIMEOUT);
    _lcd.clearScreen();
}

void DisplayHandler::updateAbsorbText(float absorb) {
    // Update period text on lcd display.
    char msg[23];
    int integer;
    int decimal;
    if (absorb < 0.0) {
        sprintf(msg,"ABSORBANCE: _.___    ");
        _lcd.print(msg,2,3);
    }
    else {
        integer = (int16_t) absorb;
        decimal = (int16_t) (1.0e3*(absorb - integer));
        sprintf(msg,"ABSORBANCE: %d.%03d    ",integer,decimal);
        _lcd.print(msg,2,3);
    }
}

void DisplayHandler::updateTracePlot(float absorb) {

    // Update the streaming plot trace.
    float scaledAbsorb;
    uint16_t displayValue;
    static uint16_t displayValueLast=0;

    if (absorb < _traceAbsorbMin) {
        absorb = _traceAbsorbMin;
    }
    if (absorb > _traceAbsorbMax) {
        absorb = _traceAbsorbMax;
    }
    scaledAbsorb = (1024*(absorb -_traceAbsorbMin)/(_traceAbsorbMax));

    // Adjust value for display
    displayValue = 55 - map((uint16_t)scaledAbsorb ,0,1024,0,35);

    if ((displayValue > 55) || (displayValue < 20)) {
        displayValue = displayValueLast;
    }
    
    // Clear old trace values
    if (_traceClearPos == 0) {
        _lcd.setPixel(_traceClearPos,_traceBuf[_traceClearPos],0);
    }
    else {
        _lcd.drawLine(
                _traceClearPos-1,
                _traceBuf[_traceClearPos-1],
                _traceClearPos,
                _traceBuf[_traceClearPos],
                0
                );
    }
    // Clear artifacts  - not sure what causes these
    _lcd.drawLine(_traceClearPos, 19, _traceClearPos, 56, 0); 

    // Plot new trace values.
    if (_tracePlotPos == 0) {
        _lcd.setPixel(_tracePlotPos,displayValue,1);
    }
    else {
        _lcd.drawLine(
                _tracePlotPos-1,
                displayValueLast,
                _tracePlotPos,
                displayValue,
                1
                );
    }

    // Save last value and add data to trace buffer.
    displayValueLast = displayValue;
    _traceBuf[_tracePlotPos] = displayValue;

    // Update trace plot position 
    _tracePlotPos += 1;
    if (_tracePlotPos > 127) {
        _tracePlotPos = 0;
    }

    // Update trace clear position
    _traceClearPos += 1;
    if (_traceClearPos > 127) {
        _traceClearPos = 0;
    }
}
