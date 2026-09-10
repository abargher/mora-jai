#include "jaios.h"
#include "../mora/mora.h"

void push_led_state(jai_board_t board, uint8_t corners);

STATE st_puzzle_callback(jaios_t* os, event_data_t evt_data) {
    //Discard "button up" events
    if (!evt_data.evt_data.button.isPressed) {
        return ST_PUZZLE;
    }

    BUTTON_NAME button = evt_data.evt_data.button.button;
    
    //For now ignore all aux buttons
    if (button >= BT_AUX_LEFT) {
        return ST_PUZZLE;
    }

    //Handle corner presses
    if (button >= BT_CORNER_TL) {
        if (!mora_win(os->puzzle.curr_state)) {
            os->puzzle.curr_state = os->puzzle.orig_state;
            os->puzzle.corners_pressed = CORNERS_RESET;
            push_led_state(os->puzzle.curr_state, CORNERS_RESET);
            return ST_PUZZLE;
        }
    }
}

void push_led_state(jai_board_t board, uint8_t corners) {

}