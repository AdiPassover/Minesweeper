#ifndef MINESWEEPER_GAME_DIFFICULTIES_H
#define MINESWEEPER_GAME_DIFFICULTIES_H

#define NUM_DIFFICULTIES 5

typedef enum {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
    NIGHTMARE = 3,
    CUSTOM = 4
} DifficultyIndex;

typedef struct {
    unsigned int rows;
    unsigned int cols;
    unsigned int mines;
} Difficulty;

extern const DifficultyIndex DEFAULT_DIFFICULTY;

extern Difficulty DIFFICULTIES[NUM_DIFFICULTIES];
extern const char* const DIFFICULTY_NAMES[NUM_DIFFICULTIES];

#endif //MINESWEEPER_GAME_DIFFICULTIES_H
