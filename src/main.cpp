#include <ncurses.h>
#include <pthread.h>
#include<stdlib.h>
#include <limits.h>
#include<stdio.h>
#include<stdbool.h>
#include<sys/stat.h>
// C++ stuffs yeah idk
#include<vector>
#include<chrono>
//#include "config.hpp"
#include "logo.hpp"

#ifdef _WIN32
#include<windows.h>
#else
#include<unistd.h>
#endif

struct loopData {
    int *inp;
    bool *new_inp;
    Logo &logo;
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
    Logo logo = data->logo;
    int frame_rate = data->frame_rate;
    std::chrono::milliseconds chrono_sleep_time(1000 / frame_rate);
    int input = -1;
    //int last_inp = -1; // most recent input
    int rows,cols = 0;
    getmaxyx(stdscr, cols, rows);
    int x_dir = -1;
    int y_dir = -1;
    int y_mag = 1, x_mag = 2; // Slope

    //int x = 20,y = 20;
    logo.pos = {20, 20};
    int times = 0;
    int fadeOut = 2 + 0; // n showing at any time
    std::vector<int> lastFiveX(fadeOut);
    std::vector<int> lastFiveY(fadeOut);
    int color = 0;
    //clear();
    bool hyper_speed = false;
    bool gay = false;
    bool debug = false;
    std::chrono::time_point<std::chrono::steady_clock> prevFrame = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> curFrame = std::chrono::steady_clock::now();
        while (true) {
        timeout(0);
        input = getch();
       //if (input != -1) {
       //    last_inp = input;
       //}
        switch (input) {
            case 113: // q
            case 3: { // ctrl-c
                move(0, 0);
                printw("uwu");
                return NULL;
            } break;
            case 104: { // h
                hyper_speed = !hyper_speed;
            } break;
            case 103: { // g
                gay = !gay;
                init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
            } break;
            case KEY_F(4): { // esc
                    debug = !debug;
            } break;
            default: {
            }
        }
        curFrame = std::chrono::steady_clock::now();
        if (curFrame - prevFrame >= chrono_sleep_time || hyper_speed) {
        getmaxyx(stdscr, cols, rows);
        //clear();
        if (debug) {
            move(0,0);
            printw("%d\n%d", input, color / 5);
        }
        if (logo.pos.y >= cols - logo.size.y) {
            y_dir = -1;
        } else if (logo.pos.y <= 0) {
            y_dir = 1;
        }
        if (logo.pos.x >= rows - logo.size.x) {
            x_dir = -1;
        } else if (logo.pos.x <= 1) {
            x_dir = 1;
        }
        logo.clear();
        logo.pos.y += y_dir * y_mag;
        logo.pos.x += x_dir * x_mag;
        if (gay) {
            color = (color % 70) + 1;
            init_pair(1, (color / 10) + 1, COLOR_BLACK);
        }
        times = (times + 1) % fadeOut;
        lastFiveX[times] = logo.pos.x;
        lastFiveY[times] = logo.pos.y;
        prevFrame = curFrame;
        logo.print();
        //clearLogo(lastFiveY[(times + 1) % fadeOut], lastFiveX[(times + 1) % fadeOut], logo_len, logo_height);
        //printLogo(logo, y, x, logo_len, logo_height);
        refresh();
        }
    }
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
    std::vector<std::string> logo = getLogoFromFile(file_path, &logo_len, &logo_height, &file_succ);
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
    Logo logo_obj(0, 0, logo_height, logo_len, logo);
    int input = INT_MIN;
    bool new_inp = true;
    int fps = 15;
    struct loopData loopData = {&input, &new_inp, logo_obj, fps};
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
    //for (int i = 0; i < logo_height; ++i) {
    //    free(logo[i]);
    //}
    //free(logo);
    //pthread_mutex_destroy(&lock);
    printf("Exiting gracefully...\n");
    return 0;
}
