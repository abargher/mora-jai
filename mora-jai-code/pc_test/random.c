#include <stdio.h>
#include "random.h"

const char *test_board = "mora_pgggyygyy_yyyy";
const char *test_rainbow = "mora_GBEYPPWRO_UGBE";

void random_tests()
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

void free_validation_results(validation_results_t *res)
{
    free(res->failed_puzzles);
    free(res->final_states);
    free(res);
}

validation_results_t *validate_all(int puzzle_count, const char *puzzles[], int solution_count, const char *solutions[])
{
    if (puzzle_count != solution_count)
    {
        printf("Cannot validate inequal puzzles and solutions! Found %d puzzles and %d solutions\n", puzzle_count, solution_count);
        return NULL;
    }
    validation_results_t *res = calloc(sizeof(validation_results_t), 1);

    res->failed_puzzles = calloc(sizeof(jai_board_t), puzzle_count);
    res->final_states = calloc(sizeof(jai_board_t), puzzle_count);

    for (int i = 0; i < puzzle_count; i++)
    {
        jai_board_t board = make_mora(puzzles[i]);
        if (board.hash_64 == INVALID_BOARD.hash_64)
        {
            printf("Invalid puzzle in list at index %d: %s\n", i, puzzles[i]);
            return NULL;
        }
        bool succ = validate_single(board, solutions[i], res->final_states + i);
        res->tested_count++;
        if (succ)
        {
            res->solved_count++;
        }
        else
        {
            res->failed_puzzles[res->failed_count] = board;
            res->failed_count++;
        }
    }

    return res;
}

bool validate_single(jai_board_t board, const char *solution, jai_board_t *final_position)
{
    int len = strlen(solution);
    for (int i = 0; i < len; i++)
    {
        int8_t move = solution[i] - '0';
        if (move < 0 || move > 8)
        {
            printf("Invalid solution at char \"%c\" in solution string: %s\n", solution[i], solution);
            return false;
        }

        board = mora_move(board, move);
    }
    *final_position = board;
    return mora_win(board);
}

bool validate_single_verbose(jai_board_t board, const char *solution, jai_board_t *final_position)
{
    int len = strlen(solution);
    printf("move _, res %s\n", mora_to_str(board));
    for (int i = 0; i < len; i++)
    {
        int8_t move = solution[i] - '0';
        if (move < 0 || move > 8)
        {
            printf("Invalid solution at char \"%c\" in solution string: %s\n", solution[i], solution);
            return false;
        }
        board = mora_move(board, move);
        printf("move %d, res %s\n", move, mora_to_str(board));
    }
    if (final_position != NULL)
    {
        *final_position = board;
    }
    return mora_win(board);
}