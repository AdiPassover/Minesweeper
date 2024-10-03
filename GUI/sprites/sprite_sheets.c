#include "sprite_sheets.h"

const int SCALE_FACTOR = 2;

const char *const FACES_PATH = "GUI/sprites/sheets/faces.png";
const char *const TILES_PATH = "GUI/sprites/sheets/tiles.png";
const char *const DIGITS_PATH = "GUI/sprites/sheets/digits.png";

const int FACE_WIDTH = 24;
const int FACE_HEIGHT = 24;

const int TILE_WIDTH = 16;
const int TILE_HEIGHT = 16;

const int DIGIT_WIDTH = 13;
const int DIGIT_HEIGHT = 23;


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

    int sprite_width = gdk_pixbuf_get_width(sprite_sheet);
    int sprite_height = gdk_pixbuf_get_height(sprite_sheet);

    if (x < 0 || y < 0 || x + width > sprite_width || y + height > sprite_height) {
        g_printerr("Sprite sheet dimensions: %d x %d\n", sprite_width, sprite_height);
        g_printerr("Cropping at (%d, %d) with size %d x %d\n", x, y, width, height);
        g_printerr("Error: Cropping coordinates out of bounds\n");
        g_object_unref(sprite_sheet);
        return NULL;
    }

    GdkPixbuf *cropped = gdk_pixbuf_new_subpixbuf(sprite_sheet, x, y, width, height);
    GdkPixbuf *scaled = gdk_pixbuf_scale_simple(cropped, width * 2, height * 2, GDK_INTERP_BILINEAR);
    GtkWidget *image = gtk_image_new_from_pixbuf(scaled);

    g_object_unref(sprite_sheet);
    g_object_unref(cropped);
    g_object_unref(scaled);

    return image;
}

GtkWidget *get_cell_image(Cell cell) {
    char cell_char = cell_to_char(cell);
    switch (cell_char) {
        case HIDDEN_CHAR:
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * HIDDEN_TILE, 0, TILE_WIDTH, TILE_HEIGHT);
        case EMPTY_REVEALED_CHAR:
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * EMPTY_REVEALED_TILE, 0, TILE_WIDTH, TILE_HEIGHT);
        case FLAGGED_CHAR:
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * FLAGGED_TILE, 0, TILE_WIDTH, TILE_HEIGHT);
        case MINE_CHAR:
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * MINE_TILE, 0, TILE_WIDTH, TILE_HEIGHT);
        case MINE_EXPLODED_CHAR:
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * MINE_EXPLODED_TILE, 0, TILE_WIDTH, TILE_HEIGHT);
        case FALSE_FLAG_CHAR:
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * FALSE_FLAG_TILE, 0, TILE_WIDTH, TILE_HEIGHT);
        default: // numbers tiles
            return get_image_from_sprite_sheet(TILES_PATH, TILE_WIDTH * (cell_char-'1'), TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
    }
}

GtkWidget *get_face_image(enum FaceIndices face) {
    return get_image_from_sprite_sheet(FACES_PATH, FACE_WIDTH * face, 0, FACE_WIDTH, FACE_HEIGHT);
}

GtkWidget *get_digit_image(int digit) {
    if (digit < 0 || digit > 9) {
        g_printerr("Error: Invalid digit %d\n", digit);
        return NULL;
    }
    return get_image_from_sprite_sheet(DIGITS_PATH, DIGIT_WIDTH * digit, 0, DIGIT_WIDTH, DIGIT_HEIGHT);
}