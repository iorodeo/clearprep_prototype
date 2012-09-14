#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "Streaming.h"
#include "constants.h"
#include "SerialLCD.h"
#include "DisplayHandler.h"
#include "Colorimeter.h"
#include "LowPassFilter.h"

SoftwareSerial softSerial(LCD_RX_PIN, LCD_TX_PIN);
DisplayHandler display(softSerial);
Colorimeter colorimeter;

void setup() {
    Serial.begin(9600);

    // Initialize colorimeter
    colorimeter.initialize();
    colorimeter.numSamples = NUMBER_OF_SAMPLES;
    colorimeter.calibration.red = RED_LED_CALIBRATION;

    // Intialize LCD display
    softSerial.begin(LCD_BAUDRATE);
    delay(LCD_INIT_DELAY);
    display.initialize();
    display.showSplashScreen();
}

void loop() {

    static LowPassFilter lowPass = LowPassFilter(LOWPASS_FREQCUT);
    static bool isFirstSample = true;
    static unsigned long lastUpdateTime = 0;
    unsigned long currentTime;
    float dt;
    float absorb;
    float absorbFilt;

    // Get measurement and update lowpass filter
    absorb = colorimeter.getAbsorbanceRed();
    if (isFirstSample) {
        isFirstSample = false;
        lowPass.setValue(absorb);
        lastUpdateTime = micros();
        absorbFilt = absorb;
    }
    else {
        currentTime = micros();
        dt = getUpdateDt(currentTime, lastUpdateTime);
        lastUpdateTime = currentTime;
        absorbFilt = lowPass.update(absorb,dt);
    }

    // Update display text and streaming data trace
    display.setBrightness();
    display.updateAbsorbText(absorbFilt);
    display.updateTracePlot(absorbFilt);
    delay(LOOP_DELAY);
}

float getUpdateDt(uint32_t t1, uint32_t t0)  {
    float dt;
    if (t1 > t0) {
        dt = t1 - t0;
    }
    else {
        dt = t1 + (UINT32_MAX - t0);
    }
    dt = 1.0e-6*dt;
    return dt;
}
