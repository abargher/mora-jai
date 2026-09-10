#include "jaios.h"
#include "main_puzzles.h"

STATE st_puzzle_solved_callback(jaios_t* os, event_data_t evt_data) {
    //Discard non-button events
    if (evt_data.evt != EVT_BUTTON) {
        return ST_PUZZLE_SOLVED;
    }
    
    //Discard "button up" events
    if (!evt_data.evt_data.button.isPressed) {
        return ST_PUZZLE_SOLVED;
    }

    if (evt_data.evt_data.button.button != BT_AUX_MID) {
        return ST_PUZZLE_SOLVED;
    }

    if (os->curr_puzzle_idx >= 91) {
        os->curr_puzzle_idx = 1;
    }
    else {
        os->curr_puzzle_idx++;
    }
    jai_board_t board = make_mora(os_test_puzzles[os->curr_puzzle_idx]);
    os->puzzle.orig_state = board;
    os->puzzle.curr_state = board;

    push_led_state(board, CORNERS_RESET);
    LatchLock();
    return ST_PUZZLE;
}