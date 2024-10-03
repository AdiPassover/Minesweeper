#include "minesweeper_gui.h"
#include "sprites/sprite_sheets.h"

#define BOARD_SIZE 16
#define NUM_MINES 40

static GtkWidget *grid;
static GtkWidget *event_box;

// Function to update the board display (this should remain as is)
void update_board_display(Board *board) {
    // Clear the grid before updating
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

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

gboolean on_grid_button_press(GtkWidget *widget, GdkEventButton *event, Board *board) {
    unsigned int row = (unsigned int)event->y / (TILE_HEIGHT * SCALE_FACTOR);
    unsigned int col = (unsigned int)event->x / (TILE_WIDTH * SCALE_FACTOR);

    if (event->type == GDK_BUTTON_PRESS) {
        if (event->button == 1) {
            printf("Left Click at row: %d, col: %d\n", row, col);
            click_cell(board, row, col);
        } else if (event->button == 3) {
            printf("Right Click at row: %d, col: %d\n", row, col);
            right_click_cell(board, row, col);
        }
        update_board_display(board);
    }

    return TRUE;
}

void initialize_gui(Board *board) {
    gtk_init(NULL, NULL);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid to hold the tiles
    grid = gtk_grid_new();

    // Create an event box and wrap the grid inside it
    event_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(event_box), grid);

    // Add the event box to the window
    gtk_container_add(GTK_CONTAINER(window), event_box);

    // Set event mask to allow button press events on the event box
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);

    // Connect the unified button press event handler to the event box
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(on_grid_button_press), board);

    // Update the board display
    update_board_display(board);

    // Show the window
    gtk_widget_show_all(window);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");
    gtk_window_set_default_size(GTK_WINDOW(window), BOARD_SIZE * TILE_WIDTH * SCALE_FACTOR,
                                BOARD_SIZE * TILE_HEIGHT * SCALE_FACTOR + FACE_HEIGHT * 2);
}

void run_gui() {
    gtk_main();
}