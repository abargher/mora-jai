#include <stdio.h>

#include "../lib/mora.h"

const char *test_board = "mora_pgggyygyy_y";
const char *test_rainbow = "mora_GBEPYVWRO_U";

int main()
{
    jai_board_t board = make_mora(test_board);
    printf("%ld\n", board.hash_64);
    printf("%s\n", print_mora(board));

    jai_board_t rainbow = make_mora(test_rainbow);
    for (int i = 0; i < 9; i++)
    {
        printf("%s\n", color_to_str(mora_index(rainbow, i)));
    }
}