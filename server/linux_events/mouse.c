#include <X11/Xlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DO_NOTHING_MODE     0;
#define GET_EVENTS_MODE     1;
#define MOVE_ONLY_MODE      2;

int mode = DO_NOTHING_MODE;
pthread_cond_t mode_condition;
pthread_mutex_t mode_mutex;

void *process_stdin(Display *display, Window window) {
    char *input;
    unsigned long len;
    ssize_t rc;
    while (true) {
        rc = getline(&input, &len, stdin);
        if (rc == -1) {
            continue;
        }
        if (input == "resume") {
            XGrabPointer(display,
                         window,
                         1,
                         PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                         GrabModeAsync,
                         GrabModeAsync,
                         None,
                         None,
                         CurrentTime);
            XGrabKeyboard(display,
                          window,
                          1,
                          GrabModeAsync,
                          GrabModeAsync,
                          CurrentTime);
            mode = GET_EVENTS_MODE;
            pthread_cond_broadcast(&mode_condition);
        }
        else if (input == "cancel" ){
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = DO_NOTHING_MODE;
            pthread_cond_broadcast(&mode_condition);
        }
        else if (input == "movement only") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = MOVE_ONLY_MODE;
            pthread_cond_broadcast(&mode_condition);
        }
        else if (input == "quit") {
            free(input);
            break;
        }
        free(input);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&mode_mutex, NULL);
    pthread_cond_init(&mode_condition, NULL);

    //main logic


    pthread_mutex_destroy(&mode_mutex);
    pthread_cond_destroy(&mode_condition);
}