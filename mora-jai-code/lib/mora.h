#include <stdint.h>
#include <stdio.h>

#ifndef MORA_H
#define MORA_H

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
    BLUE
} COLOR;

typedef struct
{
    union
    {
        struct
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
            uint8_t goal_color;
        } board;
        uint8_t bits[5];
        struct
        {
            uint32_t upper;
            uint32_t lower;
        } hash_32;
#ifdef __x86_64__
        uint64_t hash_64;
#endif
    };
} jai_board_t;

jai_board_t make_mora(const char *board);

char color_to_char(COLOR c);
COLOR char_to_color(char c);

char *color_to_str(COLOR c);

/// @brief Prints state to storage string, i.e. "mora_GBEPYVWRO_U"
/// @param board
/// @return
char *mora_to_str(jai_board_t board);

jai_board_t make_mora(const char *board);

/// @brief Prints state to a human-formated string
/// @param board
/// @return
char *print_mora(jai_board_t board);

void mora_move(jai_board_t board, uint8_t move);

static inline COLOR mora_index(jai_board_t board, uint8_t idx)
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

#endif // MORA_H