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
    int last_inp = -1; // most recent input
    int rows,cols = 0;
    getmaxyx(stdscr, cols, rows);
    int y_mag = 1, x_mag = 2; // Slope

    int times = 0;
    int fadeOut = 2 + 0; // n showing at any time
    std::vector<int> lastFiveX(fadeOut);
    std::vector<int> lastFiveY(fadeOut);
    std::vector<int> rainbow = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_BLUE, COLOR_MAGENTA};
    for (std::size_t i = 2; i < rainbow.size() + 2; i++) {
        init_pair(i, rainbow[i - 2], COLOR_BLACK);
    }
    int color = 0;
    //clear();
    bool hyper_speed = false;
    bool gay = false;
    bool debug = false;
    struct pos max_size;
    struct pos dir = {1, 1};
    std::chrono::time_point<std::chrono::steady_clock> prevFrame = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> curFrame = std::chrono::steady_clock::now();
        while (true) {
        timeout(0);
        input = getch();
       if (input != -1) {
           last_inp = input;
            switch (last_inp) {
                case 113: // q
                case 3: { // ctrl-c
                    move(0, 0);
                    return NULL;
                } break;
                case 104: { // h
                    hyper_speed = !hyper_speed;
                } break;
                case 103: { // g
                    gay = !gay;
                    logo.staticGay = false;
                    if (gay) {
                        init_pair(1, rainbow[color], COLOR_BLACK);
                    } else {
                        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
                    }
                } break;
                case 119: { // w
                    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
                    gay = false;
                    logo.staticGay = !logo.staticGay;
                } break;
                case KEY_F(4): { // esc
                        debug = !debug;
                } break;
                default: {
                }
            }
        }
        if (debug) {
            move(0,0);
            printw("%d\n%d", input, color / 5);
        }
        curFrame = std::chrono::steady_clock::now();
        if (curFrame - prevFrame >= chrono_sleep_time || hyper_speed) {
        getmaxyx(stdscr, cols, rows);
        max_size = {cols, rows};
        //clear();
        struct pos new_dir = logo.collision(max_size, dir);
        if (gay && (new_dir.x * dir.x == -1 || new_dir.y * dir.y == -1)) {
            color = (color + 1) % rainbow.size();
            init_pair(1, rainbow[color], COLOR_BLACK);
        }
        dir = new_dir;
        times = (times + 1) % fadeOut;
        lastFiveX[times] = logo.getPos().x;
        lastFiveY[times] = logo.getPos().y;
        prevFrame = curFrame;
        logo.move(dir.y * y_mag, dir.x * x_mag, {0, 0}, max_size);
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

    Logo logo_obj(0, 0, logo_height, logo_len, logo);
    int input = INT_MIN;
    bool new_inp = true;
    int fps = 15;
    struct loopData loopData = {&input, &new_inp, logo_obj, fps};
    mainLoop(&loopData);
    endwin();
    printf("Exiting gracefully...\n");
    return 0;
}
