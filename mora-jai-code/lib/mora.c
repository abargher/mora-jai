#include <stdlib.h>

#include "mora.h"

char color_to_char(COLOR c)
{
    switch (c)
    {
    case GREY:
        return 'G';
    case BLACK:
        return 'B';
    case GREEN:
        return 'E';
    case PINK:
        return 'P';
    case YELLOW:
        return 'Y';
    case VIOLET:
        return 'V';
    case WHITE:
        return 'W';
    case RED:
        return 'R';
    case ORANGE:
        return 'O';
    case BLUE:
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
        return GREY;
    case 'B':
    case 'b':
        return BLACK;
    case 'E':
    case 'e':
        return GREEN;
    case 'P':
    case 'p':
        return PINK;
    case 'Y':
    case 'y':
        return YELLOW;
    case 'V':
    case 'v':
        return VIOLET;
    case 'W':
    case 'w':
        return WHITE;
    case 'R':
    case 'r':
        return RED;
    case 'O':
    case 'o':
        return ORANGE;
    case 'U':
    case 'u':
        return BLUE;
    }
    return GREY;
}

char *color_to_str(COLOR c)
{
    switch (c)
    {
    case GREY:
        return "GREY";
    case BLACK:
        return "BLACK";
    case GREEN:
        return "GREEN";
    case PINK:
        return "PINK";
    case YELLOW:
        return "YELLOW";
    case VIOLET:
        return "VIOLET";
    case WHITE:
        return "WHITE";
    case RED:
        return "RED";
    case ORANGE:
        return "ORANGE";
    case BLUE:
        return "BLUE";
    }
    return "";
}

char *mora_to_str(jai_board_t board)
{
    char *str = calloc(17, sizeof(char));
    if (str == NULL)
    {
        return NULL;
    }
    snprintf(str, 17, "mora_%c%c%c%c%c%c%c%c%c_%c",
             color_to_char(board.board.top_left),
             color_to_char(board.board.top_center),
             color_to_char(board.board.top_right),
             color_to_char(board.board.middle_left),
             color_to_char(board.board.middle_center),
             color_to_char(board.board.middle_right),
             color_to_char(board.board.bottom_left),
             color_to_char(board.board.bottom_center),
             color_to_char(board.board.bottom_right),
             color_to_char(board.board.goal_color));
    return str;
}

jai_board_t make_mora(const char *board)
{
    jai_board_t new_board;
    char values[10];
    sscanf(board, "mora_%c%c%c%c%c%c%c%c%c_%c",
           values + 0,
           values + 1,
           values + 2,
           values + 3,
           values + 4,
           values + 5,
           values + 6,
           values + 7,
           values + 8,
           values + 9);
    new_board.board.top_left = char_to_color(values[0]);
    new_board.board.top_center = char_to_color(values[1]);
    new_board.board.top_right = char_to_color(values[2]);
    new_board.board.middle_left = char_to_color(values[3]);
    new_board.board.middle_center = char_to_color(values[4]);
    new_board.board.middle_right = char_to_color(values[5]);
    new_board.board.bottom_left = char_to_color(values[6]);
    new_board.board.bottom_center = char_to_color(values[7]);
    new_board.board.bottom_right = char_to_color(values[8]);
    new_board.board.goal_color = char_to_color(values[9]);

    return new_board;
}

char *print_mora(jai_board_t board)
{
    char *res = calloc(41, sizeof(char));
    snprintf(res, 41, "Mora Jai -- Goal: %c\n%c %c %c\n\n%c %c %c\n\n%c %c %c\n",
             color_to_char(board.board.goal_color),
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

static inline void move_grey(jai_board_t board, uint8_t move)
{
}

static inline void move_black(jai_board_t board, uint8_t move)
{
}

static inline void move_green(jai_board_t board, uint8_t move)
{
}

static inline void move_pink(jai_board_t board, uint8_t move)
{
}

static inline void move_yellow(jai_board_t board, uint8_t move)
{
}

static inline void move_violet(jai_board_t board, uint8_t move)
{
}

static inline void move_white(jai_board_t board, uint8_t move)
{
}

static inline void move_red(jai_board_t board, uint8_t move)
{
    board.board.top_center = GREEN;
}

static inline void move_orange(jai_board_t board, uint8_t move)
{
}

static inline void move_blue(jai_board_t board, uint8_t move)
{
}

void mora_move(jai_board_t board, uint8_t move)
{
    switch (mora_index(board, move))
    {
    case GREY:
        move_grey(board, move);
        return;
    case BLACK:
        move_black(board, move);
        return;
    case GREEN:
        move_green(board, move);
        return;
    case PINK:
        move_pink(board, move);
        return;
    case YELLOW:
        move_yellow(board, move);
        return;
    case VIOLET:
        move_violet(board, move);
        return;
    case WHITE:
        move_white(board, move);
        return;
    case RED:
        move_red(board, move);
        return;
    case ORANGE:
        move_orange(board, move);
        return;
    case BLUE:
        move_blue(board, move);
        return;
    }
}
