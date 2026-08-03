#include <stdio.h>

#include "../lib/mora.h"
#include "random.h"
#include "puzzles_index.h"

int main()
{
    jai_board_t test = make_mora(b12);
    validate_single_verbose(test, "5113117220610204544566", NULL);
    // return 0;

    int p_count = sizeof(puzzles) / sizeof(puzzles[0]);
    int s_count = sizeof(solutions) / sizeof(solutions[0]);
    validation_results_t *res = validate_all(p_count, puzzles, s_count, solutions);

    if (res != NULL)
    {
        printf("Valided %d puzzles, with success on %d\n", res->tested_count, res->solved_count);

        if (res->failed_count != 0)
        {
            char *puzzle = mora_to_str(res->failed_puzzles[0]);
            char *final = mora_to_str(res->final_states[7]);
            printf("First failed: puzzle %s, final state %s\n", puzzle, final);
        }
    }
}