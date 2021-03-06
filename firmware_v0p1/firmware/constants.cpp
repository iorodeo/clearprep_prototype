#include "constants.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Processing loop parameters
const int LOOP_DELAY = 50;

// Default colorimeter parameters 
const unsigned long RED_LED_CALIBRATION = 25000;
const unsigned long GREEN_LED_CALIBRATION = 25000;
const unsigned long BLUE_LED_CALIBRATION = 25000;
const unsigned int NUMBER_OF_SAMPLES = 2;

// Lowpass filter parameters
const float LOWPASS_FREQCUT = 1.0;

// LCD communication pins
const int LCD_RX_PIN = 12;
const int LCD_TX_PIN = 13;
const unsigned long LCD_BAUDRATE = 115200;
const unsigned long LCD_INIT_DELAY = 2500;

// RGB LED Control Pins
const int LED_BLU_PIN = 9; 
const int LED_GRN_PIN = 10; 
const int LED_RED_PIN = 11;

// Color Sensor Control Pins
const int COLOR_SENSOR_S0 = 4; 
const int COLOR_SENSOR_S1 = 5; 
const int COLOR_SENSOR_S2 = 6; 
const int COLOR_SENSOR_S3 = 7; 
const int COLOR_SENSOR_FO = 8;       

// Display values 
const int DISPLAY_POT_AIN = A0;
const int DISPLAY_SPLASH_TIMEOUT = 2000;
const int DISPLAY_CLEAR_OFFSET = 15;
const float DISPLAY_TRACE_MIN = 0.0;
const float DISPLAY_TRACE_MAX = 2.5;
