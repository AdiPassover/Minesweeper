#ifndef MINESWEEPER_GAME_SETTINGS_H
#define MINESWEEPER_GAME_SETTINGS_H

typedef enum {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
    NIGHTMARE = 3
} Difficulty;

extern const int NUM_ROWS_INDEX;
extern const int NUM_COLS_INDEX;
extern const int NUM_MINES_INDEX;

extern const int DIFFICULTY_MODES[4][3];

#endif //MINESWEEPER_GAME_SETTINGS_H
