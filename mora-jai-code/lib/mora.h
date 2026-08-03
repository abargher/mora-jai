#ifndef MORA_H
#define MORA_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    GREY = 0,
    BLACK,
    GREEN,
    PINK,
    YELLOW,
    VIOLET,
    WHITE,
    RED,
    ORANGE,
    BLUE,
} COLOR;

#define MORA_COLOR_COUNT 10

typedef union jai_board
{
    struct board
    {
        uint8_t top_left : 4;
        uint8_t top_center : 4;
        uint8_t top_right : 4;
        uint8_t middle_left : 4;
        uint8_t middle_center : 4;
        uint8_t middle_right : 4;
        uint8_t bottom_left : 4;
        uint8_t bottom_center : 4;
        uint8_t bottom_right : 4;
        uint8_t __unused : 4; // for byte padding lol
        uint8_t goal_1 : 4;
        uint8_t goal_2 : 4;
        uint8_t goal_3 : 4;
        uint8_t goal_4 : 4;
    } board;
    uint8_t bits[5];
    struct hash_32
    {
        uint32_t upper;
        uint32_t lower;
    } hash_32;
#ifdef __x86_64__
    uint64_t hash_64;
#endif
} jai_board_t;

extern const jai_board_t INVALID_BOARD;

jai_board_t make_mora(const char *board);

char color_to_char(COLOR c);
COLOR char_to_color(char c);

char *color_to_str(COLOR c);

/// @brief Prints state to storage string, i.e. "mora_GBEPYVWRO_UGBE"
/// @param board
/// @return
char *mora_to_str(jai_board_t board);

jai_board_t make_mora(const char *board);

/// @brief Prints state to a human-formated string
/// @param board
/// @return
char *print_mora(jai_board_t board);

jai_board_t mora_move(jai_board_t board, uint8_t move);

static inline COLOR mora_get(jai_board_t board, uint8_t idx)
{
    if (idx > 8)
    {
        return GREY;
    }
    uint8_t pair = board.bits[idx >> 1];
    if (idx & 1)
    {
        return pair >> 4;
    }
    else
    {
        return pair & 0xF;
    }
}

static inline jai_board_t mora_set(jai_board_t board, uint8_t idx, COLOR color)
{
    uint8_t pair_val = board.bits[idx >> 1];
    if (idx & 1)
    {
        pair_val &= 0x0F;
        pair_val |= (color << 4);
        board.bits[idx >> 1] = pair_val;
    }
    else
    {
        pair_val &= 0xF0;
        pair_val |= color;
        board.bits[idx >> 1] = pair_val;
    }
    return board;
}

static inline bool mora_win(jai_board_t board)
{
    return board.board.top_left == board.board.goal_1 &&
           board.board.top_right == board.board.goal_2 &&
           board.board.bottom_left == board.board.goal_3 &&
           board.board.bottom_right == board.board.goal_4;
}

#endif // MORA_H