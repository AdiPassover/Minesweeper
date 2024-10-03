#include "timer.h"
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>  // For g_idle_add()

const int CHECK_INTERVAL_MS = 50;

// Wrapper function to pass the TimerData to the callback safely
gboolean timer_callback_wrapper(gpointer data) {
    TimerData *timer_data = (TimerData *)data;
    timer_data->gui_update_callback(timer_data->timer_display, timer_data->timer_seconds);
    return FALSE;  // Return FALSE to ensure it is called only once
}

static void* timer_thread_function(void *arg) {
    TimerData *timer_data = (TimerData *)arg;
    timer_data->timer_seconds = 0;


    const int checks_per_second = 1000 / CHECK_INTERVAL_MS;

    while (timer_data->timer_running) {
        for (int i = 0; i < checks_per_second; i++) {
            if (!timer_data->timer_running)
                return NULL;
            usleep(CHECK_INTERVAL_MS * 1000);
        }
        timer_data->timer_seconds++;

        // Use g_idle_add to safely call the wrapper function with TimerData
        g_idle_add(timer_callback_wrapper, timer_data);
    }

    return NULL;
}



void init_timer(TimerData *timer_data, TimerCallback callback, GtkWidget *timer_display) {
    timer_data->timer_seconds = 0;
    timer_data->timer_running = false;
    timer_data->gui_update_callback = callback;  // Assign the callback function
    timer_data->timer_display = timer_display;   // Store the widget pointer
}

void start_timer(TimerData *timer_data) {
    if (timer_data->timer_running) return;
    timer_data->timer_running = true;
    if (pthread_create(&timer_data->timer_thread, NULL, timer_thread_function, timer_data) != 0) {
        perror("pthread_create");
        exit(1);
    }
}

void stop_timer(TimerData *timer_data) {
    if (!timer_data->timer_running) return;
    timer_data->timer_running = false;
    if (pthread_join(timer_data->timer_thread, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
}
