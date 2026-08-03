#ifndef RANDOM_TESTS_H
#define RANDOM_TESTS_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "../lib/mora.h"

void random_tests();

typedef struct validation_results
{
    int tested_count;
    int solved_count;
    int failed_count;
    jai_board_t *failed_puzzles;
    jai_board_t *final_states;

} validation_results_t;

validation_results_t *validate_all(int puzzle_count, const char *puzzles[], int solution_count, const char *solutions[]);

void free_validation_results(validation_results_t *res);

bool validate_single(jai_board_t board, const char *solution, jai_board_t *final_position);

bool validate_single_verbose(jai_board_t board, const char *solution, jai_board_t *final_position);

#endif // RANDOM_TESTS_H