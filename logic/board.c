#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void randomize_empty_board(Board *board, unsigned int clicked_row, unsigned int clicked_col);
GameState clicked_revealed_cell(Board *board, unsigned int row, unsigned int col);
GameState click_adjacent_unrevealed_cells(Board *board, unsigned int row, unsigned int col);

Board* create_board_from_difficulty(Difficulty difficulty) {
    return create_board(difficulty.rows, difficulty.cols, difficulty.mines);
}

Board *create_board(unsigned int rows, unsigned int cols, unsigned int mines) {
    Board *board;
    if ((board = malloc(sizeof(Board))) == NULL) { perror("malloc failed"); exit(EXIT_FAILURE); }
    board->rows = rows;
    board->cols = cols;
    board->is_empty = true;
    board->flags_placed = 0;

    if (mines == 0 || mines >= rows*cols) {
        fprintf(stderr, "Invalid number of mines\n");
        free(board);
        exit(EXIT_FAILURE);
    }
    board->tiles_left = rows*cols - mines;

    if ((board->cells = malloc(rows * sizeof(Cell*))) == NULL) {
        perror("malloc failed");
        free(board);
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < rows; i++) {
        board->cells[i] = malloc(cols * sizeof(Cell));
        if (board->cells[i] == NULL) {
            perror("malloc failed");
            for (unsigned int j = 0; j < i; j++) free(board->cells[j]);
            free(board->cells);
            free(board);
            exit(EXIT_FAILURE);
        }

        for (unsigned int j = 0; j < cols; j++) {
            board->cells[i][j].is_flagged = false;
            board->cells[i][j].is_revealed = false;
            board->cells[i][j].type = EMPTY;
        }
    }

    return board;
}

void free_board(Board *board) {
    for (unsigned int i = 0; i < board->rows; i++)
        free(board->cells[i]);
    free(board->cells);
    free(board);
}

void clear_board(Board *board) {
    for (unsigned int i = 0; i < board->rows; i++) {
        for (unsigned int j = 0; j < board->cols; j++) {
            board->cells[i][j].is_flagged = false;
            board->cells[i][j].is_revealed = false;
            board->cells[i][j].type = EMPTY;
        }
    }
    board->is_empty = true;
}

void update_adjacent_cells(Board *board, unsigned int row, unsigned int col) {
    for (unsigned int i = ((row == 0) ? 0 : row - 1); i <= ((row == board->rows - 1) ? board->rows - 1 : row + 1); i++) {
        for (unsigned int j = ((col == 0) ? 0 : col - 1); j <= ((col == board->cols - 1) ? board->cols - 1 : col + 1); j++) {
            if (board->cells[i][j].type != MINE)
                board->cells[i][j].type++;
        }
    }
}

void randomize_empty_board(Board *board, unsigned int clicked_row, unsigned int clicked_col) {
    if (!board->is_empty) {
        fprintf(stderr, "Can only randomize empty board\n");
        free_board(board);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    unsigned int mines_placed = 0;
    unsigned int num_mines = board->rows * board->cols - board->tiles_left;
    while (mines_placed < num_mines) {
        unsigned int row = rand() % board->rows;
        unsigned int col = rand() % board->cols;
        if (board->cells[row][col].type != MINE && row != clicked_row && col != clicked_col) {
            board->cells[row][col].type = MINE;
            mines_placed++;
            update_adjacent_cells(board, row, col);
        }
    }

    board->is_empty = false;
}

GameState click_cell(Board *board, unsigned int row, unsigned int col) {
    if (board->is_empty)
        randomize_empty_board(board, row, col);

    if (board->cells[row][col].is_flagged)
        return GAME_ONGOING;

    if (board->cells[row][col].is_revealed && board->cells[row][col].type != EMPTY)
        return clicked_revealed_cell(board, row, col);

    board->cells[row][col].is_revealed = true;
    if (board->cells[row][col].type == MINE)
        return GAME_OVER;

    board->tiles_left--;
    if (board ->tiles_left == 0)
        return GAME_WON;

    if (board->cells[row][col].type == EMPTY)
        return click_adjacent_unrevealed_cells(board, row, col);

    return GAME_ONGOING;
}

GameState click_adjacent_unrevealed_cells(Board *board, unsigned int row, unsigned int col) {
    for (unsigned int i = ((row == 0) ? 0 : row - 1); i <= ((row == board->rows - 1) ? board->rows - 1 : row + 1); i++) {
        for (unsigned int j = ((col == 0) ? 0 : col - 1); j <= ((col == board->cols - 1) ? board->cols - 1 : col + 1); j++) {
            if (board->cells[i][j].is_revealed) continue;
            GameState state = click_cell(board, i, j);
            if (state != GAME_ONGOING) return state; // TODO test to see if I can stop after GAME_WON or wait for GAME_OVER
        }
    }

    return GAME_ONGOING;
}

GameState clicked_revealed_cell(Board *board, unsigned int row, unsigned int col) {
    // Check if number of flags around cell is equal to the number it represents
    unsigned int count_flags = 0;
    for (unsigned int i = ((row == 0) ? 0 : row - 1); i <= ((row == board->rows - 1) ? board->rows - 1 : row + 1); i++) {
        for (unsigned int j = ((col == 0) ? 0 : col - 1); j <= ((col == board->cols - 1) ? board->cols - 1 : col + 1); j++)
            if (board->cells[i][j].is_flagged) count_flags++;
    }
    if (count_flags != board->cells[row][col].type) return GAME_ONGOING;

    return click_adjacent_unrevealed_cells(board, row, col);
}

void right_click_cell(Board *board, unsigned int row, unsigned int col) {
    if (!board->is_empty && !board->cells[row][col].is_revealed) {
        board->cells[row][col].is_flagged = !board->cells[row][col].is_flagged;
        board->flags_placed += (board->cells[row][col].is_flagged) ? 1 : -1;
    }
}

Cell get_cell_type(Board *board, unsigned int row, unsigned int col) {
    return board->cells[row][col];
}

void win_board(Board *board) {
    for (unsigned int i = 0; i < board->rows; i++) {
        for (unsigned int j = 0; j < board->cols; j++) {
            if (board->cells[i][j].type == MINE) {
                board->cells[i][j].is_flagged = true;
                board->flags_placed++;
            }
            else
                board->cells[i][j].is_revealed = true;
        }
    }
}

void lose_board(Board *board) {
    for (unsigned int i = 0; i < board->rows; i++) {
        for (unsigned int j = 0; j < board->cols; j++) {
            if (board->cells[i][j].type == MINE || board->cells[i][j].is_flagged)
                board->cells[i][j].is_revealed = true;
        }
    }
}

void print_board(Board *board) {
    for (unsigned int i = 0; i < board->rows; i++) {
        for (unsigned int j = 0; j < board->cols; j++)
            printf("%c ",cell_to_char(board->cells[i][j]));
        printf("\n");
    }
}