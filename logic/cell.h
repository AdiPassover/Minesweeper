#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include <stdbool.h>

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

enum {
    HIDDEN_CHAR = ' ',
    EMPTY_REVEALED_CHAR = '#',
    FLAGGED_CHAR = 'F',
    MINE_CHAR = '*',
    MINE_EXPLODED_CHAR = 'X',
    FALSE_FLAG_CHAR = 'L'
};

typedef struct {
    CellType type;
    bool is_revealed;
    bool is_flagged;
} Cell;

char cell_to_char(Cell cell);

#endif //MINESWEEPER_CELL_H
