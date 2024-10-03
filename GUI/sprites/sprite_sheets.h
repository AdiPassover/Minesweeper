#ifndef MINESWEEPER_SPRITE_SHEETS_H
#define MINESWEEPER_SPRITE_SHEETS_H

#include <gtk/gtk.h>
#include "../../logic/cell.h"

extern const int SCREEN_RATIO;

extern const char *const FACES_PATH;
extern const char *const TILES_PATH;
extern const char *const NUMBERS_PATH;

extern const int FACE_SHEET_WIDTH;
extern const int FACE_SHEET_HEIGHT;
extern const int FACE_WIDTH;
extern const int FACE_HEIGHT;

extern const int TILE_SHEET_WIDTH;
extern const int TILE_SHEET_HEIGHT;
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;

extern const int NUMBER_SHEET_WIDTH;
extern const int NUMBER_SHEET_HEIGHT;
extern const int NUMBER_WIDTH;
extern const int NUMBER_HEIGHT;

enum TileIndices {
    TILE_HIDDEN = 0,
    TILE_EMPTY_REVEALED = 1,
    TILE_FLAGGED = 2,
    TILE_MINE = 5,
    TILE_MINE_EXPLODED = 6,
    TILE_FALSE_FLAG = 7
};

GtkWidget *get_image_from_sprite_sheet(const char *path, int x, int y, int width, int height);
GtkWidget *get_cell_image(Cell cell);

#endif // MINESWEEPER_SPRITE_SHEETS_H
