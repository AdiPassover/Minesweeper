#ifndef MINESWEEPER_TIMER_H
#define MINESWEEPER_TIMER_H

#include <stdbool.h>
#include <pthread.h>
#include <gtk/gtk.h>

// Check every interval milliseconds for the timer to stop
extern const int CHECK_INTERVAL_MS;

// Callback function type for updating the GUI
typedef void (*TimerCallback)(GtkWidget *timer_display, unsigned int seconds);

typedef struct {
    unsigned int timer_seconds;
    bool timer_running;
    pthread_t timer_thread;
    TimerCallback gui_update_callback;
    GtkWidget *timer_display;
} TimerData;

void init_timer(TimerData *timer_data, TimerCallback callback, GtkWidget *timer_display);
void start_timer(TimerData *timer_data);
void stop_timer(TimerData *timer_data);

#endif // MINESWEEPER_TIMER_H
