#include <gtk/gtk.h>
#include "logic/board.h"
#include "logic/game_settings.h"
#include "GUI/sprites/sprite_sheets.h"

#define OPTIONS_BAR_HEIGHT 40


// Static variables
static Board *board;
static GtkWidget *grid;
static GtkWidget *event_box;  // Wrap the grid in an event box
static Difficulty current_difficulty;
static GtkWidget *mines_display; // Store reference to the mines display box

void clear_widget(GtkWidget *image) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(image));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

// Function to update the board display
void update_board_display() {
    clear_widget(grid);

    // Iterate through the board and update the display
    for (unsigned int row = 0; row < board->rows; row++) {
        for (unsigned int col = 0; col < board->cols; col++) {
            Cell cell = get_cell_type(board, row, col);
            GtkWidget *image = get_cell_image(cell);
            if (image) {
                gtk_grid_attach(GTK_GRID(grid), image, col, row, 1, 1);
            }
        }
    }
    // Show all widgets in the grid
    gtk_widget_show_all(grid);
}

// Function to handle difficulty selection
void on_difficulty_selected(GtkWidget *widget, gpointer data) {
    current_difficulty = GPOINTER_TO_INT(data);
    free_board(board);
    board = create_board_from_difficulty(current_difficulty);
    update_board_display();
}

// Function to handle clicks on the reset image
void on_reset_image_clicked(GtkWidget *widget, gpointer data) {
    free_board(board);
    board = create_board_from_difficulty(current_difficulty);
    update_board_display();
}

// Unified callback function for both left and right clicks
gboolean on_grid_button_press(GtkWidget *widget, GdkEventButton *event) {
    // Get the row and column of the clicked cell
    unsigned int row = (unsigned int)event->y / (TILE_HEIGHT * SCALE_FACTOR);
    unsigned int col = (unsigned int)event->x / (TILE_WIDTH * SCALE_FACTOR);

    if (event->type == GDK_BUTTON_PRESS) {
        if (event->button == 1) {
            click_cell(board, row, col);
        } else if (event->button == 3) {
            right_click_cell(board, row, col);
        }
        update_board_display();
    }

    return TRUE; // Returning TRUE means the event has been handled
}

// Function to update the mines left display
void update_mines_left_display(int mines_left) {
    clear_widget(mines_display);

    // Split the number into its digits and update the display
    if (mines_left >= 0 && mines_left <= 999) {
        int hundreds = mines_left / 100;
        int tens = (mines_left / 10) % 10;
        int units = mines_left % 10;

        // Get digit images and pack them
        gtk_box_pack_start(GTK_BOX(mines_display), get_digit_image(hundreds), FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(mines_display), get_digit_image(tens), FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(mines_display), get_digit_image(units), FALSE, FALSE, 0);
    }

    // Show all updated widgets in the mines display
    gtk_widget_show_all(mines_display);
}

// Main function
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the board
    current_difficulty = EASY;
    board = create_board_from_difficulty(current_difficulty);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold everything
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create options bar
    GtkWidget *options_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *easy_button = gtk_button_new_with_label("Easy");
    GtkWidget *medium_button = gtk_button_new_with_label("Medium");
    GtkWidget *hard_button = gtk_button_new_with_label("Hard");
    GtkWidget *nightmare_button = gtk_button_new_with_label("Nightmare");

    g_signal_connect(easy_button, "clicked", G_CALLBACK(on_difficulty_selected), GINT_TO_POINTER(EASY));
    g_signal_connect(medium_button, "clicked", G_CALLBACK(on_difficulty_selected), GINT_TO_POINTER(MEDIUM));
    g_signal_connect(hard_button, "clicked", G_CALLBACK(on_difficulty_selected), GINT_TO_POINTER(HARD));
    g_signal_connect(nightmare_button, "clicked", G_CALLBACK(on_difficulty_selected), GINT_TO_POINTER(NIGHTMARE));

    gtk_box_pack_start(GTK_BOX(options_bar), easy_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(options_bar), medium_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(options_bar), hard_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(options_bar), nightmare_button, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), options_bar, FALSE, FALSE, 0);

    // Create statistics bar
    GtkWidget *statistics_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    mines_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); // Store reference to the mines display

    // Create an event box for the reset image
    GtkWidget *reset_image_event_box = gtk_event_box_new();
    GtkWidget *reset_image = get_face_image(SMILEY_FACE); // Change to actual image path
    gtk_container_add(GTK_CONTAINER(reset_image_event_box), reset_image);

    // Connect the click event on the event box
    g_signal_connect(reset_image_event_box, "button-press-event", G_CALLBACK(on_reset_image_clicked), NULL);

    GtkWidget *timer_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Pack mine images
    for (int i = 0; i < 3; i++) {
        GtkWidget *mine_image = get_digit_image(0);
        gtk_box_pack_start(GTK_BOX(mines_display), mine_image, FALSE, FALSE, 0);
    }

    gtk_box_pack_start(GTK_BOX(statistics_bar), mines_display, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), reset_image_event_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), timer_display, FALSE, FALSE, 0);

    // Pack timer images
    for (int i = 0; i < 3; i++) {
        GtkWidget *timer_image = get_digit_image(0);
        gtk_box_pack_start(GTK_BOX(timer_display), timer_image, FALSE, FALSE, 0);
    }

    gtk_box_pack_start(GTK_BOX(vbox), statistics_bar, FALSE, FALSE, 0);

    // Create a grid to hold the tiles
    grid = gtk_grid_new();

    // Create an event box and wrap the grid inside it
    event_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(event_box), grid);

    // Add the event box to the main vertical box
    gtk_box_pack_start(GTK_BOX(vbox), event_box, TRUE, TRUE, 0);

    // Set event mask to allow button press events on the event box
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);

    // Connect the unified button press event handler to the event box
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(on_grid_button_press), NULL);

    // Update the board display
    update_board_display();
    update_mines_left_display(DIFFICULTY_MODES[current_difficulty][NUM_MINES_INDEX]);

    // Show the window
    gtk_widget_show_all(window);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper | Adi Passover");
    gtk_window_set_default_size(GTK_WINDOW(window), board->rows * TILE_WIDTH * SCALE_FACTOR,
                                board->cols * TILE_HEIGHT * SCALE_FACTOR + FACE_HEIGHT * 2 + DIGIT_HEIGHT*1.5 + OPTIONS_BAR_HEIGHT);

    gtk_main();

    // Free the board when done
    free_board(board);

    return 0;
}
