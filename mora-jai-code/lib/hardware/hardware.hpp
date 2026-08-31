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

int RegisterButtonDownCallback(BUTTON_CALLBACK handler);
int RegisterButtonUpCallback(BUTTON_CALLBACK handler);

/* RGB LED types and functions */
typedef struct
{
    uint32_t index;
    uint32_t color;
} ledUpdate_t;

void SetColorMatrix(uint16_t *colors);
void SetColorSingle(uint32_t index, uint16_t color);

#endif // __HARDWARE_H__