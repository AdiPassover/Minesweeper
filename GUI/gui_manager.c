#include "gui_manager.h"
#include "update_display.h"
#include "../logic/timer.h"
#include "sprites/sprite_sheets.h"

static GtkWidget *grid;
static GtkWidget *event_box;  // Wrap the grid in an event box
static GtkWidget *mines_display;
static GtkWidget *timer_display;
static GtkWidget *face_display;

static Board *board;
static Difficulty current_difficulty;
static TimerData timerData;


void game_over() {
    stop_timer(&timerData);
    lose_board(board);
    update_face_display(face_display, LOSE_FACE);
}

void game_won() {
    stop_timer(&timerData);
    win_board(board);
    update_face_display(face_display, WIN_FACE);
}

void update_gui_screen() {
    update_screen(board, current_difficulty.mines - board->flags_placed, grid, mines_display);
}

void reset_screen_gui() {
    if (board) free_board(board);
    board = create_board_from_difficulty(current_difficulty);
    update_face_display(face_display, SMILEY_FACE);
    update_gui_screen();
    stop_timer(&timerData);
    update_timer_display(timer_display, 0);
}

Difficulty get_custom_difficulty() {
    // TODO
    return DIFFICULTIES[EASY];
}

void on_difficulty_selected(GtkWidget *widget, gpointer data) {
    int difficulty_index = GPOINTER_TO_INT(data);

    if (difficulty_index == CUSTOM) {
        DIFFICULTIES[CUSTOM] = get_custom_difficulty();
    }

    current_difficulty = DIFFICULTIES[difficulty_index];
    reset_screen_gui();
}

// Function to handle clicks on the reset image
void on_reset_image_clicked(GtkWidget *widget, gpointer data) {
    reset_screen_gui();
}

// Unified callback function for both left and right clicks
gboolean on_grid_button_press(GtkWidget *widget, GdkEventButton *event) {
    unsigned int row = (unsigned int)event->y / (TILE_HEIGHT * SCALE_FACTOR);
    unsigned int col = (unsigned int)event->x / (TILE_WIDTH * SCALE_FACTOR);

    if (event->type == GDK_BUTTON_PRESS) {
        if (event->button == 1) {
            if (board->is_empty) start_timer(&timerData);
            GameState state = click_cell(board, row, col);

            if (state == GAME_OVER) {
                game_over();
            } else if (state == GAME_WON) {
                game_won();
            }
        } else if (event->button == 3) {
            right_click_cell(board, row, col);
        }
        update_gui_screen();
    }

    return TRUE; // Returning TRUE means the event has been handled
}

void run_minesweeper_gui(int argc, char** argv) {
    gtk_init(&argc, &argv);

    // Initialize static variables
    current_difficulty = DIFFICULTIES[DEFAULT_DIFFICULTY];
    board = create_board_from_difficulty(current_difficulty);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold everything
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create difficulties bar
    GtkWidget *difficulties_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    for (unsigned int i = 0; i < NUM_DIFFICULTIES; i++) {
        GtkWidget *button = gtk_button_new_with_label(DIFFICULTY_NAMES[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_difficulty_selected), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(difficulties_bar), button, TRUE, TRUE, 0);
    }
    gtk_box_pack_start(GTK_BOX(vbox), difficulties_bar, FALSE, FALSE, 0);

    // Create statistics bar
    GtkWidget *statistics_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Create mines display and timer display
    mines_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    timer_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Create face display as a box
    face_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Create an event box for the face image
    GtkWidget *face_event_box = gtk_event_box_new();
    g_signal_connect(face_event_box, "button-press-event", G_CALLBACK(on_reset_image_clicked), NULL);

    // Create face image
    GtkWidget *face_image = get_face_image(SMILEY_FACE);
    gtk_container_add(GTK_CONTAINER(face_event_box), face_image);  // Add the face image to the event box

    // Pack the event box into the face display box
    gtk_box_pack_start(GTK_BOX(face_display), face_event_box, FALSE, FALSE, 0);

    // Pack mine and timer images
    for (int i = 0; i < 3; i++) {
        GtkWidget *mine_image = get_digit_image(0);
        gtk_box_pack_start(GTK_BOX(mines_display), mine_image, FALSE, FALSE, 0);

        GtkWidget *timer_image = get_digit_image(0);
        gtk_box_pack_start(GTK_BOX(timer_display), timer_image, FALSE, FALSE, 0);
    }

    // Pack statistics bar
    gtk_box_pack_start(GTK_BOX(statistics_bar), mines_display, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), face_display, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), timer_display, FALSE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(vbox), statistics_bar, FALSE, FALSE, 0);

    init_timer(&timerData, update_timer_display, timer_display);

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

    // Show the window
    update_gui_screen();
    gtk_widget_show_all(window);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper | Adi Passover");

    // Start the GTK main loop
    gtk_main();

    // Free the board when done
    free_board(board);
}