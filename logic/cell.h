#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include <stdbool.h>

typedef enum {
    GAME_OVER,
    GAME_WON,
    GAME_ONGOING
} GameState;

typedef enum {
    MINE  = -1,
    EMPTY = 0,
    ONE   = 1,
    TWO   = 2,
    THREE = 3,
    FOUR  = 4,
    FIVE  = 5,
    SIX   = 6,
    SEVEN = 7,
    EIGHT = 8
} CellType;

typedef struct {
    CellType type;
    bool is_revealed;
    bool is_flagged;
} Cell;

#endif //MINESWEEPER_CELL_H
