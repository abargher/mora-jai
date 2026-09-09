#include <stdlib.h>
#include <stdio.h>

#include "../lib/hardware/hardware.hpp"

#define MAX_BUTTON_CALLBACKS 16

BUTTON_CALLBACK up_callbacks[MAX_BUTTON_CALLBACKS];
BUTTON_CALLBACK down_callbacks[MAX_BUTTON_CALLBACKS];

bool latch_closed = false;

BUTTON_COLOR button_colors[9];
BUTTON_COLOR goal_colors[4];

bool register_callbacks(BUTTON_CALLBACK handler, BUTTON_CALLBACK list[]) {
    for (int i = 0; i < MAX_BUTTON_CALLBACKS; i++) {
        if (list[i] == NULL) {
            list[i] = handler;
            return true;
        }
    }
    return false;
}

bool RegisterButtonDownCallback(BUTTON_CALLBACK handler) { 
    return register_callbacks(handler, down_callbacks); 
}
bool RegisterButtonUpCallback(BUTTON_CALLBACK handler) {
    return register_callbacks(handler, up_callbacks);
}
// TODO: define error values for not enough space, mutex wait timeout, etc

bool unregister_callbacks(BUTTON_CALLBACK handler, BUTTON_CALLBACK list[]) {
    bool res = false;
    for (int i = 0; i < MAX_BUTTON_CALLBACKS; i++) {
        if (list[i] == handler) {
            list[i] = NULL;
            res = true;
        }
    }
    return res;
}

bool UnregisterButtonDownCallback(BUTTON_CALLBACK handler) {
    return unregister_callbacks(handler, down_callbacks);
}
bool UnregisterButtonUpCallback(BUTTON_CALLBACK handler) {
    return unregister_callbacks(handler, up_callbacks);
}

void SetupServo() {}
void SetupEventLocks() {}

/* 
 * Note that for now, PC emulation will not separately handle button down
 * and button up events
*/
void ExecuteCallbacks(buttonUpdate_t data) {
    BUTTON_CALLBACK* list;
    if (data.isPressed) {
        list = down_callbacks;
    } else {
        list = up_callbacks;
    }
    for (int i = 0; i < MAX_BUTTON_CALLBACKS; i++) {
        if (list[i] != NULL) {
            list[i](data);
        }
    }
}

//These methods are not part of the interace for OS simulation
void SetRGBMatrix(uint32_t *colors) {}
void SetRGBSingle(uint32_t index, uint32_t color) {}

void SetColorMatrix(BUTTON_COLOR *colors) {
    //Just assuming that the colors array is exactly 13 elements long I guess
    for (int i = 0; i < 9; i++) {
        button_colors[i] = colors[i];
    }
    for (int i = 0; i < 4; i++) {
        goal_colors[i] = colors[i + 9];
    }
}
void SetColorSingle(uint32_t index, BUTTON_COLOR color) {
    if (index < 9) {
        button_colors[index] = color;
    }
    else if (index < 13) {
        button_colors[index - 9] = color;
    }
    else {
        return;
    }
}

#define VALIDATE_LATCH_OPEN() \
{ if(latch_closed == false) { printf("[JAI_OS WARN] Tried to open latch\
 while already open\n");}}

#define VALIDATE_LATCH_CLOSED() \
{ if(latch_closed == true) { printf("[JAI_OS WARN] Tried to close latch\
 while already closed\n");}}

void LatchUnlock() { VALIDATE_LATCH_CLOSED(); latch_closed = false; }
void LatchLock() { VALIDATE_LATCH_OPEN(); latch_closed = true; }
void ForceLatchUnlock() { VALIDATE_LATCH_CLOSED(); latch_closed = false; }
void ForceLatchLock() { VALIDATE_LATCH_OPEN(); latch_closed = true;}

uint32_t GetBatteryMilliVolts() { return 0; }
uint32_t GetBatteryPercentage() { return 100; }
BATTERY_STATE GetBatteryState() { return CABLE_ONLY; }

LATCH_STATE GetLastLatchState() {
    if (latch_closed) {
        return LATCH_LOCKED;
    }
    return LATCH_OPEN;
}