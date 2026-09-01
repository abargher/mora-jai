#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <stdint.h>
#include <stdbool.h>

/* Button input types and functions */
typedef struct
{
    uint32_t buttonNum;
    bool isPressed; // pressed = true, released = false;
} buttonUpdate_t;

typedef void (*BUTTON_CALLBACK)(buttonUpdate_t data);

/*
Both return the index ("id") of the registered callback,
between 0 and MAX_BTN_CALLBACKS. Can be used to unregister the callback later.
Returns a negative value if there was an error in registration.
*/
int RegisterButtonDownCallback(BUTTON_CALLBACK handler);
int RegisterButtonUpCallback(BUTTON_CALLBACK handler);
// TODO: define error values for not enough space, mutex wait timeout, etc

/*
Input must be between 0 and MAX_BTN_CALLBACKS. Unregisters the corresponding
callback function from the set.
*/
int UnregisterButtonDownCallback(uint32_t index);
int UnregisterButtonUpCallback(uint32_t index);

void SetupEventLocks();
void ExecuteCallbacks(buttonUpdate_t data);

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
} BtnColor_t;

/* RGB LED types and functions */
typedef struct
{
    uint32_t index;
    uint32_t color;
} ledUpdate_t;

// TODO: logically couple the LEDs for each button together
// e.g., there are only 13 addressable LED zones
void SetRGBMatrix(uint32_t *colors);
void SetRGBSingle(uint32_t index, uint32_t color);

void SetColorMatrix(BtnColor_t *colors);
void SetColorSingle(uint32_t index, BtnColor_t color);

/* Latch (servo) functions */

void LatchUnlock();
void LatchLock();

/* Hardware info functions */
// report battery charge level
// report battery charging state (read STAT_1 for currently charging, STAT_2 for finished)
// report if running on USB power (read PWR_GOOD)
// report if latch is open or closed (read latch state file value)

/* Display functions */
// write text to screen
// .... more to come

#endif // __HARDWARE_H__