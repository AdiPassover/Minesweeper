#include <gtk/gtk.h>
#include "logic/board.h"
#include "GUI/sprites/sprite_sheets.h"

#define BOARD_SIZE 16
#define NUM_MINES 40

static Board *board;
static GtkWidget *grid;

// Function to update the board display
void update_board_display() {
    for (unsigned int row = 0; row < BOARD_SIZE; row++) {
        for (unsigned int col = 0; col < BOARD_SIZE; col++) {
            Cell cell = get_cell_type(board, row, col);
            GtkWidget *image = get_cell_image(cell);
            if (image) {
                gtk_grid_attach(GTK_GRID(grid), image, col, row, 1, 1);
            }
        }
    }
    gtk_widget_show_all(grid);
}

// Callback function for left click
void on_left_click(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        printf("x: %f, y: %f\n", event->x, event->y);
        unsigned int row = (unsigned int)event->y / (TILE_SHEET_HEIGHT * 2);
        unsigned int col = (unsigned int)event->x / (TILE_SHEET_WIDTH * 2);
        click_cell(board, row, col);
        update_board_display();
    }
}

// Callback function for right click
void on_right_click(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        unsigned int row = (unsigned int)event->y / (TILE_SHEET_HEIGHT * 2);
        unsigned int col = (unsigned int)event->x / (TILE_SHEET_WIDTH * 2);
        right_click_cell(board, row, col);
        update_board_display();
    }
}

// Main function
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the board
    board = create_board(BOARD_SIZE, BOARD_SIZE, NUM_MINES);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid to hold the tiles
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Connect the left and right click events
    g_signal_connect(grid, "button-press-event", G_CALLBACK(on_left_click), NULL);
    g_signal_connect(grid, "button-press-event", G_CALLBACK(on_right_click), NULL);

    // Update the board display
    update_board_display();

    // Show the window
    gtk_widget_show_all(window);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");
    gtk_window_set_default_size(GTK_WINDOW(window), BOARD_SIZE * TILE_SHEET_WIDTH * 2, BOARD_SIZE * TILE_SHEET_HEIGHT * 2 + FACE_SHEET_HEIGHT * 2);

    gtk_main();

    // Free the board when done
    free_board(board);

    return 0;
}
