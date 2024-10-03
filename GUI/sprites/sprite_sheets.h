#ifndef MINESWEEPER_SPRITE_SHEETS_H
#define MINESWEEPER_SPRITE_SHEETS_H

#include <gtk/gtk.h>
#include "../../logic/cell.h"

extern const int SCALE_FACTOR;

extern const char *const FACES_PATH;
extern const char *const TILES_PATH;
extern const char *const DIGITS_PATH;

extern const int FACE_WIDTH;
extern const int FACE_HEIGHT;

extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;

extern const int DIGIT_WIDTH;
extern const int DIGIT_HEIGHT;

GtkWidget *get_image_from_sprite_sheet(const char *path, int x, int y, int width, int height);

enum TileIndices {
    HIDDEN_TILE = 0,
    EMPTY_REVEALED_TILE = 1,
    FLAGGED_TILE = 2,
    MINE_TILE = 5,
    MINE_EXPLODED_TILE = 6,
    FALSE_FLAG_TILE = 7
};
GtkWidget *get_cell_image(Cell cell);

enum FaceIndices {
    SMILEY_FACE = 0,
    SMILEY_FACE_PRESSED = 1,
    SURPRISED_FACE = 2,
    WIN_FACE = 3,
    LOSE_FACE = 4
};
GtkWidget *get_face_image(enum FaceIndices);

GtkWidget *get_digit_image(int digit);

#endif // MINESWEEPER_SPRITE_SHEETS_H
