#ifndef __PINS_H__
#define __PINS_H__

/* Change pin definitions here if board pinout differs from reference design. */

#define LED_PIN LED_BUILTIN

#define MUX_SIGNAL D6
#define MUX_CH0 D0
#define MUX_CH1 D1
#define MUX_CH2 D2
#define MUX_CH3 D3

#define LED_DATA_PIN D10
#define NUM_PIXELS 22

#define SERVO_CTRL_PIN D9

#define BATT_V_PIN A8
#define BATT_V_R1_VALUE 10000
#define BATT_V_R2_VALUE 2200

#define CHARG_ON_PIN D7
#define CHARG_DONE_PIN 42
#define PWR_GOOD_PIN 41

#define SDA_PIN SDA
#define SCL_PIN SCL

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

/*
Breadboard pin definitions:

#define LED_PIN LED_BUILTIN

#define MUX_SIGNAL D3
#define MUX_CH0 D1
#define MUX_CH1 D2
#define MUX_CH2 D0
#define MUX_CH3 D6

#define LED_DATA_PIN D7
#define NUM_PIXELS 22

#define SERVO_CTRL_PIN D8

*/

#endif /* __PINS_H__ */