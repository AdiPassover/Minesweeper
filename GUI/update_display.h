#ifndef MINESWEEPER_UPDATE_DISPLAY_H
#define MINESWEEPER_UPDATE_DISPLAY_H

#include "../logic/board.h"
#include "sprites/sprite_sheets.h"
#include <gtk/gtk.h>

// updates the display of the minesweeper grid
void update_board_display(GtkWidget *grid, Board *board);

// updates the display of the number of mines left
void update_mines_left_display(GtkWidget *mines_left_display, int mines_left);

// updates the display of the timer (called automatically every second)
void update_timer_display(GtkWidget *timer_display, unsigned int seconds);

// updates the display of the face button
void update_face_display(GtkWidget *face_display, enum FaceIndices face);

// updates the entire screen
void update_screen(Board *board, int mines_left, GtkWidget *grid, GtkWidget *mines_left_display);


#endif //MINESWEEPER_UPDATE_DISPLAY_H
