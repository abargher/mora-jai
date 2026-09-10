#include "jaios.h"
#include "../mora/mora.h"

bool check_puzzle_corner(jai_board_t board, BUTTON_NAME corner);

STATE st_puzzle_callback(jaios_t* os, event_data_t evt_data) {
    //Discard non-button events
    if (evt_data.evt != EVT_BUTTON) {
        return ST_PUZZLE;
    }

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
        if (!check_puzzle_corner(os->puzzle.curr_state, button)) {
            os->puzzle.curr_state = os->puzzle.orig_state;
            os->puzzle.corners_pressed = CORNERS_RESET;
            push_led_state(os->puzzle.curr_state, CORNERS_RESET);
            return ST_PUZZLE;
        }
        else {
            //Button press was ok
            os->puzzle.corners_pressed |= 1 << (button - BT_CORNER_TL);
            if (os->puzzle.corners_pressed == CORNERS_SOLVED) {
                //Double check puzzle solved
                if (!mora_win(os->puzzle.curr_state)) {
                    os->puzzle.curr_state = os->puzzle.orig_state;
                    os->puzzle.corners_pressed = CORNERS_RESET;
                    push_led_state(os->puzzle.curr_state, CORNERS_RESET);
                    return ST_PUZZLE;
                }
                //Puzzle is definitely solved
                push_led_state(os->puzzle.curr_state, CORNERS_SOLVED);
                LatchUnlock();
                return ST_PUZZLE_SOLVED;
            }

            push_led_state(os->puzzle.curr_state, os->puzzle.corners_pressed);
        }
    }

    //Handle button presses
    jai_board_t new_board = mora_move(os->puzzle.curr_state, button);
    os->puzzle.curr_state = new_board;
    push_led_state(new_board, os->puzzle.corners_pressed);
    return ST_PUZZLE;
}

void push_led_state(jai_board_t board, uint8_t corners) {
    BUTTON_COLOR colors[13];
    for (int i = 0; i < 9; i++) {
        colors[i] = color_to_hardware(mora_get(board, i), false);
    }
    colors[9] = color_to_hardware(board.board.goal_1, corners & (0b0001));
    colors[10] = color_to_hardware(board.board.goal_2, corners & (0b0010));
    colors[11] = color_to_hardware(board.board.goal_3, corners & (0b0100));
    colors[12] = color_to_hardware(board.board.goal_4, corners & (0b1000));

    SetColorMatrix(colors);
}

bool check_puzzle_corner(jai_board_t board, BUTTON_NAME corner) {
    switch (corner) {
        case BT_CORNER_TL:
            return board.board.top_left == board.board.goal_1;
        case BT_CORNER_TR:
            return board.board.top_right == board.board.goal_2;
        case BT_CORNER_BL:
            return board.board.bottom_left == board.board.goal_3;
        case BT_CORNER_BR:
            return board.board.bottom_right == board.board.goal_4;
        default:
            return false;
    }
}