#include <ncurses.h>
#include <pthread.h>
#include<stdlib.h>
#include <limits.h>
#include<stdio.h>
#include<stdbool.h>
#include<sys/stat.h>
//#include "config.h"
#include "logo.h"

#ifdef _WIN32
#include<windows.h>

#define SLEEP_FUNC Sleep
#define SLEEP_TIME 1000 // Sleep -> milliseconds
#else
#include<unistd.h> // NO WINDOWS ONLY LINUX
#define SLEEP_FUNC usleep
#define SLEEP_TIME 1000000 // usleep -> microseconds
#endif

struct loopData {
    int *inp;
    bool *new_inp;
    char **logo;
    int logo_len;
    int logo_height;
    int frame_rate;
};



void *mainLoop(void *inp) {
    initscr();
    curs_set(0);
    //cbreak();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);

    struct loopData *data = (struct loopData *) inp;
    char **logo = data->logo;
    //int *input = data->inp;
    //bool *new_inp = data->new_inp;
    int logo_len = data->logo_len;
    int logo_height = data->logo_height;
    int frame_rate = data->frame_rate;
    int sleep_time = SLEEP_TIME / frame_rate; 
    int input = -1;
    int last_inp = -1; // most recent input
    int rows,cols = 0;
    getmaxyx(stdscr, cols, rows);
    int x_dir = -1;
    int y_dir = -1;
    int y_mag = 1, x_mag = 2; // Slope

    int x = 20,y = 20;
    SLEEP_FUNC(sleep_time);
    int times = 0;
    int fadeOut = 2 + 0; // n showing at any time
    int *lastFiveX = calloc(fadeOut, sizeof(int));
    int *lastFiveY = calloc(fadeOut, sizeof(int));
    int color = 0;
    //clear();
    bool hyper_speed = false;
    bool debug = false;
        while (true) {
        timeout(0);
        input = getch();
        if (input != -1) {
            last_inp = input;
        }
        //*input = getch();
        switch (input) {
            case 113: // Q
            case 3: { // ctrl-c
                //endwin();
                move(0, 0);
                printw("uwu");
                free(lastFiveX);
                free(lastFiveY);
                return NULL;
            } break;
            case 104: { // h
                    hyper_speed = !hyper_speed;
            } break;
            case KEY_F(4): { // esc
                    debug = !debug;
            } break;
            default: {
            }
        }
        getmaxyx(stdscr, cols, rows);
        //clearLogo(lastFiveY[(times + 1) % fadeOut], lastFiveX[(times + 1) % fadeOut], logo_len, logo_height);
        clear();
        printLogo(logo, y, x, logo_len, logo_height);
        if (debug) {
            move(0,0);
            printw("%d\n%d", input, color / 5);
        }
        if (y >= cols - logo_height) {
            y_dir = -1;
        } else if (y <= 0) {
            y_dir = 1;
        }
        if (x >= rows - logo_len) {
            x_dir = -1;
        } else if (x <= 1) {
            x_dir = 1;
        }
        y += y_dir * y_mag;
        x += x_dir * x_mag;
        color = (color % 70) + 1;
        init_pair(1, (color / 10) + 1, COLOR_BLACK);
        times = (times + 1) % fadeOut;
        lastFiveX[times] = x;
        lastFiveY[times] = y;
        //if (times != 1) {
        refresh();
        //}
        if (!hyper_speed) {
            SLEEP_FUNC(sleep_time);
        }
    }
    free(lastFiveY);
    free(lastFiveX);
    return NULL;
}

int main(int argc, char *argv[0]) {
    if (argc < 2) {
        printf("Usage: %s [ASCII PATH]\n", argv[0]);
        return 1;
    }
    char *file_path = argv[1];

    initscr();
    if (has_colors() == false) {
        endwin();
        printf("L you don't have colours\n");
        return 1;
    }
    endwin();
    bool file_succ;
    int logo_len = 0;
    int logo_height = 0;
    char **logo = getLogoFromFile(file_path, &logo_len, &logo_height, &file_succ);
    if (!file_succ) {
        endwin();
        printf("File %s couldn't be opened.\n", file_path);
        return 1;
    }

    //pthread_t main_loop;

    //if (pthread_mutex_init(&lock, NULL) != 0) {
    //   printf("\nMutex init failed :(\n");
    //    return 1;
    //}
    int input = INT_MIN;
    bool new_inp = true;
    struct loopData loopData = {&input, &new_inp, logo, logo_len, logo_height, 15};
    //pthread_create(&main_loop, NULL, mainLoop, &loopData);
    mainLoop(&loopData);
   // while (input != 3) {
   //     input = getch();
   //     pthread_mutex_lock(&lock);
   //     new_inp = true;
   //     pthread_mutex_unlock(&lock);
   // }
    //pthread_join(main_loop, NULL);
    endwin();
    //free(logo);
    for (int i = 0; i < logo_height; ++i) {
        free(logo[i]);
    }
    free(logo);
    //pthread_mutex_destroy(&lock);
    printf("Exiting gracefully...\n");
    return 0;
}
