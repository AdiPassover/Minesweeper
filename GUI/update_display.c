#include "update_display.h"

void clear_widget(GtkWidget *image) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(image));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void update_board_display(GtkWidget *grid, Board *board) {
    clear_widget(grid);

    for (unsigned int row = 0; row < board->rows; row++) {
        for (unsigned int col = 0; col < board->cols; col++) {
            Cell cell = get_cell_type(board, row, col);
            GtkWidget *image = get_cell_image(cell);
            if (image) {
                gtk_grid_attach(GTK_GRID(grid), image, col, row, 1, 1);
            }
        }
    }

    gtk_widget_show_all(grid);
}

typedef struct {
    int units;
    int tens;
    int hundreds;
} Digits;

Digits split_number(int number) {
    Digits digits;
    digits.units = number % 10;
    digits.tens = (number / 10) % 10;
    digits.hundreds = number / 100;
    return digits;
}

void update_mines_left_display(GtkWidget *mines_left_display, int mines_left) {
    clear_widget(mines_left_display);

    if (mines_left < 0) mines_left = 0;
    if (mines_left > 999) mines_left = 999;
    Digits digits = split_number(mines_left);

    gtk_box_pack_start(GTK_BOX(mines_left_display), get_digit_image(digits.hundreds), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mines_left_display), get_digit_image(digits.tens), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mines_left_display), get_digit_image(digits.units), FALSE, FALSE, 0);

    gtk_widget_show_all(mines_left_display);
}

void update_timer_display(GtkWidget *timer_display, unsigned int timer_seconds) {
    clear_widget(timer_display);

    unsigned int seconds = timer_seconds;
    if (seconds > 999) seconds = 999;
    Digits digits = split_number(seconds);

    gtk_box_pack_start(GTK_BOX(timer_display), get_digit_image(digits.hundreds), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(timer_display), get_digit_image(digits.tens), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(timer_display), get_digit_image(digits.units), FALSE, FALSE, 0);

    gtk_widget_show_all(timer_display);
}


void update_face_display(GtkWidget *face_display, enum FaceIndices face) {
    GtkWidget *event_box = g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(face_display)), 0);
    GtkWidget *current_image = g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(event_box)), 0);
    if (current_image)
        gtk_widget_destroy(current_image);

    GtkWidget *new_face_image = get_face_image(face);
    gtk_container_add(GTK_CONTAINER(event_box), new_face_image);

    gtk_widget_show_all(face_display);
}


void update_screen(Board *board, int mines_left, GtkWidget *grid, GtkWidget *mines_left_display) {
    update_board_display(grid, board);
    update_mines_left_display(mines_left_display, mines_left);
}