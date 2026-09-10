#include <stdlib.h>

#include "mora.h"

const jai_board_t INVALID_BOARD = {.hash_32 = {.upper = ~0, .lower = ~0}};

char color_to_char(COLOR c)
{
    switch (c)
    {
    case MORA_GRAY:
        return 'G';
    case MORA_BLACK:
        return 'B';
    case MORA_GREEN:
        return 'E';
    case MORA_PINK:
        return 'P';
    case MORA_YELLOW:
        return 'Y';
    case MORA_VIOLET:
        return 'V';
    case MORA_WHITE:
        return 'W';
    case MORA_RED:
        return 'R';
    case MORA_ORANGE:
        return 'O';
    case MORA_BLUE:
        return 'U';
    }
    return ' ';
}

COLOR char_to_color(char c)
{
    switch (c)
    {
    case 'G':
    case 'g':
        return MORA_GRAY;
    case 'B':
    case 'b':
        return MORA_BLACK;
    case 'E':
    case 'e':
        return MORA_GREEN;
    case 'P':
    case 'p':
        return MORA_PINK;
    case 'Y':
    case 'y':
        return MORA_YELLOW;
    case 'V':
    case 'v':
        return MORA_VIOLET;
    case 'W':
    case 'w':
        return MORA_WHITE;
    case 'R':
    case 'r':
        return MORA_RED;
    case 'O':
    case 'o':
        return MORA_ORANGE;
    case 'U':
    case 'u':
        return MORA_BLUE;
    }
    return MORA_GRAY;
}

char *color_to_str(COLOR c)
{
    switch (c)
    {
    case MORA_GRAY:
        return "GRAY";
    case MORA_BLACK:
        return "BLACK";
    case MORA_GREEN:
        return "GREEN";
    case MORA_PINK:
        return "PINK";
    case MORA_YELLOW:
        return "YELLOW";
    case MORA_VIOLET:
        return "VIOLET";
    case MORA_WHITE:
        return "WHITE";
    case MORA_RED:
        return "RED";
    case MORA_ORANGE:
        return "ORANGE";
    case MORA_BLUE:
        return "BLUE";
    }
    return "";
}

char *mora_to_str(jai_board_t board)
{
    char *str = calloc(20, sizeof(char));
    if (str == NULL)
    {
        return NULL;
    }
    snprintf(str, 20, "mora_%c%c%c%c%c%c%c%c%c_%c%c%c%c",
             color_to_char(board.board.top_left),
             color_to_char(board.board.top_center),
             color_to_char(board.board.top_right),
             color_to_char(board.board.middle_left),
             color_to_char(board.board.middle_center),
             color_to_char(board.board.middle_right),
             color_to_char(board.board.bottom_left),
             color_to_char(board.board.bottom_center),
             color_to_char(board.board.bottom_right),
             color_to_char(board.board.goal_1),
             color_to_char(board.board.goal_2),
             color_to_char(board.board.goal_3),
             color_to_char(board.board.goal_4));
    return str;
}

jai_board_t make_mora(const char *board)
{
    jai_board_t new_board;
    char values[13];
    int res = sscanf(board, "mora_%c%c%c%c%c%c%c%c%c_%c%c%c%c",
                     values + 0,
                     values + 1,
                     values + 2,
                     values + 3,
                     values + 4,
                     values + 5,
                     values + 6,
                     values + 7,
                     values + 8,
                     values + 9,
                     values + 10,
                     values + 11,
                     values + 12);
    if (res != 13)
    {
        return INVALID_BOARD;
    }
    new_board.board.top_left = char_to_color(values[0]);
    new_board.board.top_center = char_to_color(values[1]);
    new_board.board.top_right = char_to_color(values[2]);
    new_board.board.middle_left = char_to_color(values[3]);
    new_board.board.middle_center = char_to_color(values[4]);
    new_board.board.middle_right = char_to_color(values[5]);
    new_board.board.bottom_left = char_to_color(values[6]);
    new_board.board.bottom_center = char_to_color(values[7]);
    new_board.board.bottom_right = char_to_color(values[8]);
    new_board.board.goal_1 = char_to_color(values[9]);
    new_board.board.goal_2 = char_to_color(values[10]);
    new_board.board.goal_3 = char_to_color(values[11]);
    new_board.board.goal_4 = char_to_color(values[12]);

    return new_board;
}

char *print_mora(jai_board_t board)
{
    char *res = calloc(41, sizeof(char));
    snprintf(res, 41, "Mora Jai -- Goal: %c\n%c %c %c\n\n%c %c %c\n\n%c %c %c\n",
             color_to_char(board.board.goal_1),
             color_to_char(board.board.top_left),
             color_to_char(board.board.top_center),
             color_to_char(board.board.top_right),
             color_to_char(board.board.middle_left),
             color_to_char(board.board.middle_center),
             color_to_char(board.board.middle_right),
             color_to_char(board.board.bottom_left),
             color_to_char(board.board.bottom_center),
             color_to_char(board.board.bottom_right));
    return res;
}

#pragma region Move Helpers
/// @brief Swap gray <--> ANY at given location
/// @param board
/// @param move
/// @return
static inline jai_board_t _swap_ANY_gray(jai_board_t board, uint8_t move, COLOR target)
{
    COLOR temp = mora_get(board, move);
    if (temp == target)
    {
        return mora_set(board, move, MORA_GRAY);
    }
    else if (temp == MORA_GRAY)
    {
        return mora_set(board, move, target);
    }
    return board;
}
/* @brief Iterate neighbors (counter-clockwise, from top) of given idex
 *
 * 0 1 2
 *
 * 3 4 5
 *
 * 6 7 8
 */
static int8_t ortho_neighbors[][4] = {
    {3, 1, -1, -1}, // 0
    {4, 2, 0, -1},  // 1
    {5, 1, -1, -1}, // 2
    {6, 4, 0, -1},  // 3
    {3, 7, 5, 1},   // 4
    {4, 8, 2, -1},  // 5
    {7, 3, -1, -1}, // 6
    {6, 8, 4, -1},  // 7
    {7, 5, -1, -1}  // 8
};
static int8_t neighbors[][8] = {
    {1, 3, 4, -1, -1, -1, -1, -1}, // 0
    {3, 4, 5, 2, 0, -1, -1, -1},   // 1
    {4, 5, 1, -1, -1, -1, -1, -1}, // 2
    {6, 7, 4, 1, 0, -1, -1, -1},   // 3
    {3, 6, 7, 8, 5, 2, 1, 0},      // 4
    {4, 7, 8, 2, 1, -1, -1, -1},   // 5
    {7, 4, 3, -1, -1, -1, -1, -1}, // 6
    {6, 8, 5, 4, 3, -1, -1, -1},   // 7
    {7, 5, 4, -1, -1, -1, -1, -1}  // 8
};

#define ITER_ORTHO_NEIGHBORS(board, move, __iter__)                                 \
    uint8_t __iter__##_idx = 0;                                                     \
    int8_t __iter__ = 0;                                                            \
    while (__iter__##_idx < 4 && (__iter__ = ortho_neighbors[move][__iter__##_idx], \
                                  __iter__##_idx++,                                 \
                                  __iter__ != -1))

#define ITER_NEIGHBORS(board, move, __iter__)                                 \
    uint8_t __iter__##_idx = 0;                                               \
    int8_t __iter__ = 0;                                                      \
    while (__iter__##_idx < 8 && (__iter__ = neighbors[move][__iter__##_idx], \
                                  __iter__##_idx++,                           \
                                  __iter__ != -1))

#pragma endregion

#pragma region Moves
/// @brief Does nothing
/// @param board
/// @param move
static inline jai_board_t move_gray(jai_board_t board, uint8_t move)
{
    return board;
}

/// @brief Shift entire row to the right
/// @param board
/// @param move
static inline jai_board_t move_black(jai_board_t board, uint8_t move)
{
    uint8_t row_start = 3 * (move / 3);
    COLOR temp = mora_get(board, row_start + 2);
    board = mora_set(board, row_start + 2, mora_get(board, row_start + 1));
    board = mora_set(board, row_start + 1, mora_get(board, row_start));
    board = mora_set(board, row_start, temp);
    return board;
}

/// @brief Swap the selected tile with its opposite on the board
/// @param board
/// @param move
/// @return
static inline jai_board_t move_green(jai_board_t board, uint8_t move)
{
    /*
    The board is laid out
        0 1 2
        3 4 5
        6 7 8
    Note that the opposite of idx i is just 8 - i.
    We avoid casework by also swapping 4 with itself
    */

    uint8_t other_idx = 8 - move;
    COLOR temp = mora_get(board, move);
    board = mora_set(board, move, mora_get(board, other_idx));
    board = mora_set(board, other_idx, temp);
    return board;
}

/// @brief All adjacent tiles rotate clockwise around the pressed tiles
/// @param board
/// @param move
/// @return
static inline jai_board_t move_pink(jai_board_t board, uint8_t move)
{
    uint8_t first_idx = -1;
    COLOR first_color;
    uint8_t prev_idx;
    ITER_NEIGHBORS(board, move, i)
    {
        if (first_idx == (uint8_t)-1)
        {
            first_idx = i;
            first_color = mora_get(board, i);
        }
        else
        {
            board = mora_set(board, prev_idx, mora_get(board, i));
        }
        prev_idx = i;
    }
    board = mora_set(board, prev_idx, first_color);
    return board;
}

/// @brief If not on top row, swap tile with tile directly above
/// @param board
/// @param move
/// @return
static inline jai_board_t move_yellow(jai_board_t board, uint8_t move)
{
    if (move < 3)
    {
        return board;
    }
    COLOR prev = mora_get(board, move);
    board = mora_set(board, move, mora_get(board, move - 3));
    board = mora_set(board, move - 3, prev);
    return board;
}

/// @brief If not on bottom row, swap tile with tile directly below
/// @param board
/// @param move
/// @return
static inline jai_board_t move_violet(jai_board_t board, uint8_t move)
{
    if (move > 5)
    {
        return board;
    }
    COLOR prev = mora_get(board, move);
    board = mora_set(board, move, mora_get(board, move + 3));
    board = mora_set(board, move + 3, prev);
    return board;
}

/// @brief Swap gray <--> white at given location and ortho adjacents
/// @param board
/// @param move
/// @return
static inline jai_board_t move_white(jai_board_t board, uint8_t move)
{
    COLOR self_color = mora_get(board, move);
    board = mora_set(board, move, MORA_GRAY);
    ITER_ORTHO_NEIGHBORS(board, move, i)
    {
        board = _swap_ANY_gray(board, i, self_color);
    }
    return board;
}

/// @brief Turn all black tiles red, and all white tiles black
/// @param board
/// @param move
/// @return
static inline jai_board_t move_red(jai_board_t board, uint8_t move)
{
    // Need to know if pressed tile was red or blue
    COLOR red = mora_get(board, move);
    for (int i = 0; i < 9; i++)
    {
        COLOR curr = mora_get(board, i);
        if (curr == MORA_WHITE)
        {
            board = mora_set(board, i, MORA_BLACK);
        }
        else if (curr == MORA_BLACK)
        {
            board = mora_set(board, i, red);
        }
    }
    return board;
}

/// @brief Become majority color of neighbors
/// @param board
/// @param move
/// @return
static inline jai_board_t move_orange(jai_board_t board, uint8_t move)
{
    uint8_t color_counts[MORA_COLOR_COUNT] = {0};
    COLOR best = MORA_COLOR_COUNT;
    ITER_ORTHO_NEIGHBORS(board, move, i)
    {
        COLOR col = mora_get(board, i);
        color_counts[col]++;
        if (color_counts[col] == 2)
        {
            if (best == MORA_COLOR_COUNT)
            {
                best = col;
            }
            else
            {
                // We found multiple colors with 2+, must be a 2-2 tie
                return board;
            }
        }
    }
    if (best == MORA_COLOR_COUNT)
    {
        // No majority
        return board;
    }
    return mora_set(board, move, best);
}

/// @brief If center is not blue, perform action as if move is color of center
/// @param board
/// @param move
/// @return
static inline jai_board_t move_blue(jai_board_t board, uint8_t move)
{
    COLOR center = mora_get(board, 4);
    if (center == MORA_BLUE)
    {
        return board;
    }
    switch (center)
    {
    case MORA_GRAY:
        return move_gray(board, move);
    case MORA_BLACK:
        return move_black(board, move);
    case MORA_GREEN:
        return move_green(board, move);
    case MORA_PINK:
        return move_pink(board, move);
    case MORA_YELLOW:
        return move_yellow(board, move);
    case MORA_VIOLET:
        return move_violet(board, move);
    case MORA_WHITE:
        return move_white(board, move);
    case MORA_RED:
        return move_red(board, move);
    case MORA_ORANGE:
        return move_orange(board, move);
    default:
        return board;
    }
}

jai_board_t mora_move(jai_board_t board, uint8_t move)
{
    switch (mora_get(board, move))
    {
    case MORA_GRAY:
        return move_gray(board, move);
    case MORA_BLACK:
        return move_black(board, move);
    case MORA_GREEN:
        return move_green(board, move);
    case MORA_PINK:
        return move_pink(board, move);
    case MORA_YELLOW:
        return move_yellow(board, move);
    case MORA_VIOLET:
        return move_violet(board, move);
    case MORA_WHITE:
        return move_white(board, move);
    case MORA_RED:
        return move_red(board, move);
    case MORA_ORANGE:
        return move_orange(board, move);
    case MORA_BLUE:
        return move_blue(board, move);
    }
    return board;
}
// #pragma endregion