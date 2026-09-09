#include <stdbool.h>

typedef enum {
    ST_PUZZLE,
    ST_PUZZLE_SOLVE,
    ST_PUZZLE_PACK_BROWSE
} STATE;

typedef struct {
    STATE state;
    bool is_locked;
} jaios_t;

jaios_t* init();