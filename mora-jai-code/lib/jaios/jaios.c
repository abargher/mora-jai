#include <stdlib.h>
#include <string.h>

#include "jaios.h"

const STATE_CALLBACK callbacks[3] = {
    st_puzzle_callback,
    st_puzzle_solve_callback,
    st_puzzle_pack_browse_callback
};


void init(jaios_t* os) {
    if (!os) {
        return;    
    }

    memset(os, 0, sizeof(jaios_t));

    os->state = ST_PUZZLE;

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