#include <stdlib.h>
#include <string.h>

#include "jaios.h"

const STATE_CALLBACK callbacks[3] = {
    st_puzzle_callback,
    st_puzzle_solved_callback,
    st_puzzle_pack_browse_callback
};


void init(jaios_t* os) {
    if (!os) {
        return;    
    }

    memset(os, 0, sizeof(jaios_t));

    os->state = ST_PUZZLE_SOLVED;
    os->curr_puzzle_idx = 0;

    switch (GetLastLatchState()) {
        case LATCH_LOCKED:
        case LATCH_UNKNOWN:
            LatchUnlock();
            break;
        default:
            break;
    }
    os->is_locked = false;

    return;
}

void on_button_pressed(void* ctx, button_update_t button) {
    event_data_t evt;
    evt.evt = EVT_BUTTON;
    evt.evt_data.button = button;

    process_event((jaios_t*)ctx, evt);
}

void process_event(jaios_t* os, event_data_t evt) {
    os->state = callbacks[os->state](os, evt);
}

BUTTON_COLOR color_to_hardware(COLOR color, bool preview) {
    if (preview) {
        switch(color) {
            case GREY:
                return BTN_GREY;
            case BLACK:
                return BTN_BLACK_PREVIEW;
            case GREEN:
                return BTN_GREEN_PREVIEW;
            case PINK:
                return BTN_PINK_PREVIEW;
            case YELLOW:
                return BTN_YELLOW_PREVIEW;
            case VIOLET:
                return BTN_VIOLET_PREVIEW;
            case WHITE:
                return BTN_WHITE_PREVIEW;
            case RED:
                return BTN_RED_PREVIEW;
            case ORANGE:
                return BTN_ORANGE_PREVIEW;
            case BLUE:
                return BTN_BLUE_PREVIEW;
        }
    }
    switch(color) {
        case GREY:
            return BTN_GREY;
        case BLACK:
            return BTN_BLACK;
        case GREEN:
            return BTN_GREEN;
        case PINK:
            return BTN_PINK;
        case YELLOW:
            return BTN_YELLOW;
        case VIOLET:
            return BTN_VIOLET;
        case WHITE:
            return BTN_WHITE;
        case RED:
            return BTN_RED;
        case ORANGE:
            return BTN_ORANGE;
        case BLUE:
            return BTN_BLUE;
    }
    return BTN_GREY;
}