#ifndef JAIOS_H
#define JAIOS_H

#include <stdbool.h>
#include <stdint.h>

#include <mora.h>
#include <hardware.hpp>

typedef enum
{
    ST_PUZZLE,
    ST_PUZZLE_SOLVED,
    ST_PUZZLE_PACK_BROWSE
} STATE;

typedef enum
{
    EVT_BUTTON,
    EVT_TIMER_EXPIRE,
    EVT_BATTERY,
    // TBD
} EVENT;

typedef struct
{
    EVENT evt;
    union
    {
        button_update_t button;
    } evt_data;
} event_data_t;

// Per-state state structs
#define CORNERS_SOLVED 0b1111
#define CORNERS_RESET 0
typedef struct
{
    jai_board_t orig_state;
    jai_board_t curr_state;

    uint8_t corners_pressed;
} st_puzzle_t;

typedef struct
{
    STATE state;
    bool is_locked;

    // TEMP vars
    uint8_t curr_puzzle_idx;

    st_puzzle_t puzzle;
} jaios_t;

// State callbacks
typedef STATE (*STATE_CALLBACK)(jaios_t *os, event_data_t evt_data);

STATE st_puzzle_callback(jaios_t *os, event_data_t evt_data);
STATE st_puzzle_solved_callback(jaios_t *os, event_data_t evt_data);
STATE st_puzzle_pack_browse_callback(jaios_t *os, event_data_t evt_data);

extern const STATE_CALLBACK callbacks[3];

void jaios_init(jaios_t *os);

void on_button_pressed(void *ctx, button_update_t button);

void process_event(jaios_t *os, event_data_t evt);

// OS helper functions

BUTTON_COLOR color_to_hardware(COLOR color, bool preview);

void push_led_state(jai_board_t board, uint8_t corners);

#endif