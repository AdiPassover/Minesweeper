#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cell.h"

typedef struct { // TODO add a flag counter
    unsigned int rows;
    unsigned int cols;
    unsigned int tiles_left;
    bool is_empty;
    Cell **cells;
} Board;


Board* create_board(unsigned int rows, unsigned int cols, unsigned int mines);
void free_board(Board *board);

void clear_board(Board *board);
void reveal_board(Board *board);
void reveal_mines(Board *board);

GameState click_cell(Board *board, unsigned int row, unsigned int col);
void right_click_cell(Board *board, unsigned int row, unsigned int col);

Cell get_cell_type(Board *board, unsigned int row, unsigned int col);

#endif //MINESWEEPER_BOARD_H
