#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "SerialLCD.h"
#include "Streaming.h"

#include "constants.h"
#include "display_handler.h"
#include "system_state.h"
//#include "rgb_led.h"
//#include "color_sensor.h"
//#include "colorimeter.h"
//#include "lowpass_filter.h"

SystemState sysState;

void setup()
{
    sysState.initialize();
}

void loop()
{
    sysState.update();
    delay(100);
}




