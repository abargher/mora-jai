#include <stdlib.h>
#include <string.h>

#include "jaios.h"

const STATE_CALLBACK callbacks[3] = {
    st_puzzle_callback,
    st_puzzle_solved_callback,
    st_puzzle_pack_browse_callback};

void jaios_init(jaios_t *os)
{
    if (!os)
    {
        return;
    }
    RegisterButtonDownCallback(on_button_pressed);
    RegisterButtonUpCallback(on_button_pressed);

    memset(os, 0, sizeof(jaios_t));

    os->state = ST_PUZZLE_SOLVED;
    os->curr_puzzle_idx = 0;

    switch (GetLastLatchState())
    {
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

void on_button_pressed(void *ctx, button_update_t button)
{
    event_data_t evt;
    evt.evt = EVT_BUTTON;
    evt.evt_data.button = button;

    process_event((jaios_t *)ctx, evt);
}

void process_event(jaios_t *os, event_data_t evt)
{
    os->state = callbacks[os->state](os, evt);
}

BUTTON_COLOR color_to_hardware(COLOR color, bool preview)
{
    if (preview)
    {
        switch (color)
        {
        case MORA_GRAY:
            return BTN_GRAY;
        case MORA_BLACK:
            return BTN_BLACK_PREVIEW;
        case MORA_GREEN:
            return BTN_GREEN_PREVIEW;
        case MORA_PINK:
            return BTN_PINK_PREVIEW;
        case MORA_YELLOW:
            return BTN_YELLOW_PREVIEW;
        case MORA_VIOLET:
            return BTN_VIOLET_PREVIEW;
        case MORA_WHITE:
            return BTN_WHITE_PREVIEW;
        case MORA_RED:
            return BTN_RED_PREVIEW;
        case MORA_ORANGE:
            return BTN_ORANGE_PREVIEW;
        case MORA_BLUE:
            return BTN_BLUE_PREVIEW;
        }
    }
    switch (color)
    {
    case MORA_GRAY:
        return BTN_GRAY;
    case MORA_BLACK:
        return BTN_BLACK;
    case MORA_GREEN:
        return BTN_GREEN;
    case MORA_PINK:
        return BTN_PINK;
    case MORA_YELLOW:
        return BTN_YELLOW;
    case MORA_VIOLET:
        return BTN_VIOLET;
    case MORA_WHITE:
        return BTN_WHITE;
    case MORA_RED:
        return BTN_RED;
    case MORA_ORANGE:
        return BTN_ORANGE;
    case MORA_BLUE:
        return BTN_BLUE;
    }
    return BTN_GRAY;
}