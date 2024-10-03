#include "sprite_sheets.h"

const int SCREEN_RATIO = 2;

const char *const FACES_PATH = "GUI/sprites/sheets/faces.png";
const char *const TILES_PATH = "GUI/sprites/sheets/tiles.png";
const char *const NUMBERS_PATH = "GUI/sprites/sheets/numbers.png";

const int FACE_SHEET_WIDTH = 24;
const int FACE_SHEET_HEIGHT = 24;
const int FACE_WIDTH = FACE_SHEET_WIDTH * SCREEN_RATIO;
const int FACE_HEIGHT = FACE_SHEET_HEIGHT * SCREEN_RATIO;

const int TILE_SHEET_WIDTH = 16;
const int TILE_SHEET_HEIGHT = 16;
const int TILE_WIDTH = TILE_SHEET_WIDTH * SCREEN_RATIO;
const int TILE_HEIGHT = TILE_SHEET_HEIGHT * SCREEN_RATIO;

const int NUMBER_SHEET_WIDTH = 13;
const int NUMBER_SHEET_HEIGHT = 23;
const int NUMBER_WIDTH = NUMBER_SHEET_WIDTH * SCREEN_RATIO;
const int NUMBER_HEIGHT = NUMBER_SHEET_HEIGHT * SCREEN_RATIO;


GtkWidget *get_image_from_sprite_sheet(const char *path, int x, int y, int width, int height) {
    if (!g_file_test(path, G_FILE_TEST_EXISTS)) {
        g_printerr("Error: File %s not found.\n", path);
        return NULL;
    }
    GdkPixbuf *sprite_sheet = gdk_pixbuf_new_from_file(path, NULL);
    if (!sprite_sheet) {
        g_printerr("Error: Could not load sprite sheet %s.\n", path);
        return NULL;
    }

    GdkPixbuf *cropped = gdk_pixbuf_new_subpixbuf(sprite_sheet, x, y, width, height);

    // Scale the cropped pixbuf to double the size (2x width and 2x height)
    GdkPixbuf *scaled = gdk_pixbuf_scale_simple(cropped, width * 2, height * 2, GDK_INTERP_BILINEAR);
    GtkWidget *image = gtk_image_new_from_pixbuf(scaled);

    // Free the resources
    g_object_unref(sprite_sheet);
    g_object_unref(cropped);
    g_object_unref(scaled);

    return image;
}


GtkWidget *get_cell_image(Cell cell) { // TODO differentiate between win/lose
    if (!cell.is_revealed) {
        if (!cell.is_flagged) { // hidden empty cell
            return get_image_from_sprite_sheet(TILES_PATH, TILE_SHEET_WIDTH * TILE_HIDDEN, 0, TILE_SHEET_WIDTH, TILE_SHEET_HEIGHT);
        } else { // flagged cell
            return get_image_from_sprite_sheet(TILES_PATH, TILE_SHEET_WIDTH * TILE_FLAGGED, 0, TILE_SHEET_WIDTH, TILE_SHEET_HEIGHT);
        }
    } else {
        if (cell.type == MINE) {
            return get_image_from_sprite_sheet(TILES_PATH, TILE_SHEET_WIDTH * TILE_MINE, 0, TILE_SHEET_WIDTH, TILE_SHEET_HEIGHT);
        } else if (cell.type == EMPTY) {
            return get_image_from_sprite_sheet(TILES_PATH, TILE_SHEET_WIDTH * TILE_EMPTY_REVEALED, 0, TILE_SHEET_WIDTH, TILE_SHEET_HEIGHT);
        } else {
            return get_image_from_sprite_sheet(TILES_PATH, TILE_SHEET_WIDTH * cell.type, TILE_SHEET_HEIGHT, TILE_SHEET_WIDTH, TILE_SHEET_HEIGHT);
        }
    }
}