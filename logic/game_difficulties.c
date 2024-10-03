#include "game_difficulties.h"

 const DifficultyIndex DEFAULT_DIFFICULTY = EASY;

 Difficulty DIFFICULTIES[NUM_DIFFICULTIES] = {
        { 9, 9, 10 },
        { 16, 16, 40 },
        { 16, 30, 99 },
        { 16, 30, 200 },
        { 0, 0, 0 }
};

const char* const DIFFICULTY_NAMES[NUM_DIFFICULTIES] = {
        "Easy",
        "Medium",
        "Hard",
        "Nightmare",
        "Custom"
};