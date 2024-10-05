#include "difficulty_dialog_box.h"

#include "difficulty_dialog_box.h"

// Function to handle the dialog response and validate user input
static gboolean validate_and_get_input(GtkWidget *rows_entry, GtkWidget *cols_entry, GtkWidget *mines_entry, Difficulty *custom_difficulty) {
    const char *rows_text = gtk_entry_get_text(GTK_ENTRY(rows_entry));
    const char *cols_text = gtk_entry_get_text(GTK_ENTRY(cols_entry));
    const char *mines_text = gtk_entry_get_text(GTK_ENTRY(mines_entry));

    int rows = atoi(rows_text);
    int cols = atoi(cols_text);
    int mines = atoi(mines_text);

    if (rows > 0 && cols > 8 && mines >= 0 && mines < rows * cols) {
        custom_difficulty->rows = rows;
        custom_difficulty->cols = cols;
        custom_difficulty->mines = mines;
        return TRUE;
    }
    return FALSE;
}

Difficulty open_difficulty_dialog_box(GtkWidget *parent) {
    GtkWidget *dialog, *content_area;
    GtkWidget *rows_label, *cols_label, *mines_label;
    GtkWidget *rows_entry, *cols_entry, *mines_entry;
    Difficulty custom_difficulty = DIFFICULTIES[CUSTOM];

    // Create a dialog with OK and Cancel buttons
    dialog = gtk_dialog_new_with_buttons("Custom Difficulty",
                                         GTK_WINDOW(parent),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "_OK", GTK_RESPONSE_OK,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    rows_label = gtk_label_new("Rows:");
    cols_label = gtk_label_new("Columns:");
    mines_label = gtk_label_new("Mines:");

    rows_entry = gtk_entry_new();
    cols_entry = gtk_entry_new();
    mines_entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(rows_entry), g_strdup_printf("%d", custom_difficulty.rows));
    gtk_entry_set_text(GTK_ENTRY(cols_entry), g_strdup_printf("%d", custom_difficulty.cols));
    gtk_entry_set_text(GTK_ENTRY(mines_entry), g_strdup_printf("%d", custom_difficulty.mines));

    gtk_grid_attach(GTK_GRID(grid), rows_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rows_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), cols_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), cols_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mines_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mines_entry, 1, 2, 1, 1);

    gtk_box_pack_start(GTK_BOX(content_area), grid, TRUE, TRUE, 0);
    gtk_widget_show_all(dialog);

    // Run the dialog and check the response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_OK ||
        !validate_and_get_input(rows_entry,cols_entry, mines_entry, &custom_difficulty))
    {
        gtk_widget_destroy(dialog);
        return DIFFICULTIES[DEFAULT_DIFFICULTY];
    }

    gtk_widget_destroy(dialog);

    return custom_difficulty;
}
