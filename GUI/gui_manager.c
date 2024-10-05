#include "gui_manager.h"
#include "update_display.h"
#include "../logic/timer.h"
#include "difficulty_dialog_box.h"
#include "sprites/sprite_sheets.h"

const char* BACKGROUND_COLOR = "#c8c4c4";
const int BUTTON_HEIGHT = 25;

static GtkWidget *window;
static GtkWidget *statistics_bar;
static GtkWidget *grid;
static GtkWidget *grid_event_box;
static GtkWidget *mines_display;
static GtkWidget *timer_display;
static GtkWidget *face_display;

static Board *board;
static Difficulty current_difficulty;
static TimerData timerData;
static GameState current_state;


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
    stop_timer(&timerData);
    if (board) free_board(board);
    board = create_board_from_difficulty(current_difficulty);
    current_state = GAME_ONGOING;
    update_face_display(face_display, SMILEY_FACE);
    update_gui_screen();
    update_timer_display(timer_display, 0);
}

void on_difficulty_selected(GtkWidget *widget, gpointer data) {
    int difficulty_index = GPOINTER_TO_INT(data);

    if (difficulty_index == CUSTOM) {
        DIFFICULTIES[CUSTOM] = open_difficulty_dialog_box(window);
    }

    current_difficulty = DIFFICULTIES[difficulty_index];
    reset_screen_gui();

    int grid_width = board->cols * TILE_WIDTH * SCALE_FACTOR;
    int grid_height = board->rows * TILE_HEIGHT * SCALE_FACTOR;

    int total_height = grid_height + gtk_widget_get_allocated_height(statistics_bar) + BUTTON_HEIGHT;

    gtk_window_resize(GTK_WINDOW(window), grid_width, total_height);

    gtk_widget_show_all(window);
}

// Function to handle clicks on the reset image
void on_reset_image_clicked(GtkWidget *widget, gpointer data) {
    reset_screen_gui();
}

// Unified callback function for both left and right clicks
gboolean on_grid_button_press(GtkWidget *widget, GdkEventButton *event) {
    if (current_state != GAME_ONGOING) return TRUE;

    unsigned int row = (unsigned int)event->y / (TILE_HEIGHT * SCALE_FACTOR);
    unsigned int col = (unsigned int)event->x / (TILE_WIDTH * SCALE_FACTOR);

    if (event->type == GDK_BUTTON_PRESS) {
        if (event->button == 1) {
            if (board->is_empty) start_timer(&timerData);
            current_state = click_cell(board, row, col);

            if (current_state == GAME_OVER) {
                game_over();
            } else if (current_state == GAME_WON) {
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

    current_difficulty = DIFFICULTIES[DEFAULT_DIFFICULTY];
    board = create_board_from_difficulty(current_difficulty);
    current_state = GAME_ONGOING;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold everything
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create and apply CSS to set the background color and button style
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    // Use the constant BACKGROUND_COLOR
    gchar *css_data = g_strdup_printf(
            "window, event-box { background-color: %s; }"
            "button { background-color: %s; min-width: 40px; min-height: %dpx; padding: 3px; }",
            BACKGROUND_COLOR, BACKGROUND_COLOR, BUTTON_HEIGHT
    );
    gtk_css_provider_load_from_data(cssProvider, css_data, -1, NULL);
    g_free(css_data);

    // Apply CSS provider to the window
    GtkStyleContext *styleContext = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *difficulties_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    for (unsigned int i = 0; i < NUM_DIFFICULTIES; i++) {
        GtkWidget *button = gtk_button_new_with_label(DIFFICULTY_NAMES[i]);
        GtkStyleContext *buttonStyleContext = gtk_widget_get_style_context(button);
        gtk_style_context_add_provider(buttonStyleContext, GTK_STYLE_PROVIDER(cssProvider),
                                       GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_signal_connect(button, "clicked", G_CALLBACK(on_difficulty_selected), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(difficulties_bar), button, FALSE, TRUE, 0);
    }
    gtk_box_pack_start(GTK_BOX(vbox), difficulties_bar, FALSE, FALSE, 0);

    statistics_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    mines_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    timer_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    face_display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *face_event_box = gtk_event_box_new();
    g_signal_connect(face_event_box, "button-press-event", G_CALLBACK(on_reset_image_clicked), NULL);

    GtkWidget *face_image = get_face_image(SMILEY_FACE);
    gtk_container_add(GTK_CONTAINER(face_event_box), face_image);  // Add the face image to the event box
    gtk_box_pack_start(GTK_BOX(face_display), face_event_box, FALSE, FALSE, 0);

    for (int i = 0; i < 3; i++) {
        GtkWidget *mine_image = get_digit_image(0);
        gtk_box_pack_start(GTK_BOX(mines_display), mine_image, FALSE, FALSE, 0);

        GtkWidget *timer_image = get_digit_image(0);
        gtk_box_pack_start(GTK_BOX(timer_display), timer_image, FALSE, FALSE, 0);
    }

    GtkWidget *left_spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *right_spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_box_pack_start(GTK_BOX(statistics_bar), mines_display, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), left_spacer, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), face_display, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(statistics_bar), right_spacer, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(statistics_bar), timer_display, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), statistics_bar, FALSE, FALSE, 0);

    init_timer(&timerData, update_timer_display, timer_display);

    grid = gtk_grid_new();

    // Create an event box and wrap the grid inside it
    grid_event_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(grid_event_box), grid);
    gtk_box_pack_start(GTK_BOX(vbox), grid_event_box, TRUE, TRUE, 0);

    // Apply the background color to the event box
    GtkStyleContext *eventBoxStyleContext = gtk_widget_get_style_context(grid_event_box);
    gtk_style_context_add_provider(eventBoxStyleContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_add_events(grid_event_box, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(grid_event_box, "button-press-event", G_CALLBACK(on_grid_button_press), NULL);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    // Show the window
    update_gui_screen();
    gtk_widget_show_all(window);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");

    // Start the GTK main loop
    gtk_main();

    // Free the board when done
    free_board(board);
}
