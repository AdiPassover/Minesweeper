#include "game_settings.h"

const int NUM_ROWS_INDEX = 0;
const int NUM_COLS_INDEX = 1;
const int NUM_MINES_INDEX = 2;

const int DIFFICULTY_MODES[4][3] = {
        {9, 9, 10},
        {16, 16, 40},
        {16, 30, 99},
        {20, 30, 130}
};