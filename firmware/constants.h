#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// Processing loop parameters
extern const int LOOP_DELAY;

// Colorimeter parameters
extern const unsigned long RED_LED_CALIBRATION;
extern const unsigned long GREEN_LED_CALIBRATION;
extern const unsigned long BLUE_LED_CALIBRATION;
extern const unsigned int NUMBER_OF_SAMPLES;

// Lowpass filter parameters
extern const float LOWPASS_FREQCUT;

// LCD communication pins
extern const int LCD_RX_PIN;
extern const int LCD_TX_PIN;
extern const unsigned long LCD_BAUDRATE;
extern const unsigned long LCD_INIT_DELAY;

// RGB LED Control Pins
extern const int LED_BLU_PIN; 
extern const int LED_GRN_PIN; 
extern const int LED_RED_PIN;

// Color Sensor Control Pins
extern const int COLOR_SENSOR_S0; 
extern const int COLOR_SENSOR_S1; 
extern const int COLOR_SENSOR_S2; 
extern const int COLOR_SENSOR_S3; 
extern const int COLOR_SENSOR_FO;       

// Display values
enum {DISPLAY_TRACE_BUF_LEN = 128};
extern const int DISPLAY_POT_AIN;
extern const int DISPLAY_SPLASH_TIMEOUT;
extern const int DISPLAY_CLEAR_OFFSET;
extern const float DISPLAY_TRACE_MIN;
extern const float DISPLAY_TRACE_MAX;

#endif

