#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cpluspluss
extern "C" {
#endif

typedef enum {
    BT_FACE_0,
    BT_FACE_1,
    BT_FACE_2,
    BT_FACE_3,
    BT_FACE_4,
    BT_FACE_5,
    BT_FACE_6,
    BT_FACE_7,
    BT_FACE_8,

    BT_CORNER_TL,
    BT_CORNER_TR,
    BT_CORNER_BL,
    BT_CORNER_BR,

    BT_AUX_LEFT,
    BT_AUX_MID,
    BT_AUX_RIGHT,
} BUTTON_NAME;

/* Button input types and functions */
typedef struct
{
    BUTTON_NAME button;
    bool isPressed; // pressed = true, released = false;
} button_update_t;

typedef void (*BUTTON_CALLBACK)(void* ctx, button_update_t data);

/*
Returns TRUE on success, else FALSE. Registered C++ functions MUST be declared with extern "C"
*/
bool RegisterButtonDownCallback(BUTTON_CALLBACK handler);

/*
Returns TRUE on success, else FALSE. Registered C++ functions MUST be declared with extern "C"
*/
bool RegisterButtonUpCallback(BUTTON_CALLBACK handler);
// TODO: define error values for not enough space, mutex wait timeout, etc

/*
Returns TRUE on success, else FALSE
*/
bool UnregisterButtonDownCallback(BUTTON_CALLBACK handler);
bool UnregisterButtonUpCallback(BUTTON_CALLBACK handler);

void SetupServo();
void SetupEventLocks();
void ExecuteCallbacks(button_update_t data);

// TODO: determine and assign values for all colors
typedef enum
{
    BTN_GRAY = 0x3A3A3A,
    BTN_WHITE = 0xFFFFFF,
    BTN_VIOLET = 0x000000,
    BTN_YELLOW = 0xFFFF00,
    BTN_GREEN = 0x00FF00,
    BTN_BLACK = 0x000000,
    BTN_RED = 0xFF0000,
    BTN_PINK = 0x000000,
    BTN_ORANGE = 0x000000,
    BTN_BLUE = 0x0000FF,

    BTN_WHITE_PREVIEW = 0xFFFFFF,
    BTN_VIOLET_PREVIEW = 0x000000,
    BTN_YELLOW_PREVIEW = 0xFFFF00,
    BTN_GREEN_PREVIEW = 0x00FF00,
    BTN_BLACK_PREVIEW = 0x000000,
    BTN_RED_PREVIEW = 0xFF0000,
    BTN_PINK_PREVIEW = 0x000000,
    BTN_ORANGE_PREVIEW = 0x000000,
    BTN_BLUE_PREVIEW = 0x0000FF,
} BUTTON_COLOR;

/* RGB LED types and functions */
typedef struct
{
    uint32_t index;
    uint32_t color;
} ledUpdate_t;

// TODO: logically couple the LEDs for each button together
// e.g., there are only 13 addressable LED zones
// Match the button orders with order in jai_board_t:
// 0-8: face buttons, left to right, top to bottom
// 9: top left
// 10: top right
// 11: bottom right
// 12: bottom left
void SetRGBMatrix(uint32_t *colors);
void SetRGBSingle(uint32_t index, uint32_t color);

void SetColorMatrix(BUTTON_COLOR *colors);
void SetColorSingle(uint32_t index, BUTTON_COLOR color);

/* Latch (servo) functions */

void LatchUnlock();
void LatchLock();
void ForceLatchUnlock();
void ForceLatchLock();

typedef enum
{
    NO_CABLE,    // USB cable is not plugged in, battery discharging
    CABLE_ONLY,  // USB cable is plugged in, but battery does not seem connected
    CHARGING,    // Battery is currently charging
    CHARGE_DONE, // Battery has finished charging
    ERROR        // we are in some error state (both STAT1 and STAT2 are low)
} BATTERY_STATE;

typedef enum
{
    LATCH_OPEN,
    LATCH_LOCKED,
    LATCH_UNKNOWN
} LATCH_STATE;

/* Hardware info functions */
uint32_t GetBatteryMilliVolts();
uint32_t GetBatteryPercentage();
BATTERY_STATE GetBatteryState();

LATCH_STATE GetLastLatchState();

/* Display functions */
// write text to screen
// .... more to come

#ifdef _cplusplus
}
#endif
#endif // __HARDWARE_H__