#include <stdio.h>

#include "../lib/mora.h"

const char *test_board = "mora_pgggyygyy_yyyy";
const char *test_rainbow = "mora_GBEYPPWRO_UGBE";

int main()
{
    jai_board_t board = make_mora(test_board);
    printf("%ld\n", board.hash_64);
    printf("%s\n", print_mora(board));

    board = mora_set(board, 0, BLUE);
    board = mora_set(board, 1, VIOLET);
    board = mora_set(board, 2, BLACK);

    printf("%s\n", print_mora(board));

    board = mora_move(board, 2);

    printf("%s\n", print_mora(board));

    jai_board_t rainbow = make_mora(test_rainbow);
    for (int i = 0; i < 9; i++)
    {
        printf("%s\n", color_to_str(mora_get(rainbow, i)));
    }

    rainbow = mora_move(rainbow, 1);
    rainbow = mora_move(rainbow, 2);
    rainbow = mora_move(rainbow, 1);
    rainbow = mora_move(rainbow, 4);
    rainbow = mora_move(rainbow, 7);
    printf("%s\n", print_mora(rainbow));
    rainbow = mora_move(rainbow, 3);
    printf("%s\n", print_mora(rainbow));
}