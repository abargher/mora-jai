#include <stdlib.h>

#include "jaios.h"
#include "..\hardware\hardware.hpp"

jaios_t* init() {
    jaios_t* os = calloc(sizeof(jaios_t), 1);

    if (!os) {
        return NULL;    
    }

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

    return os;
}