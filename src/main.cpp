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
#include<iostream>
#include "config.hpp"
#include "logo.hpp"

#ifdef _WIN32
#include<windows.h>
#else
#include<unistd.h>
#endif

enum {
    PURPLE_PAIR = 1
};

void *mainLoop(Logo &logo, Config &config) {
    // ncurses stuff
    initscr();
    curs_set(0);
    //cbreak();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(PURPLE_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(PURPLE_PAIR));

    int frame_rate = config.fps;
    std::chrono::milliseconds chrono_sleep_time(1000 / frame_rate);
    int input = -1;
    int last_inp = -1; // most recent input
    int rows,cols = 0;
    getmaxyx(stdscr, cols, rows);
    int y_mag = 1, x_mag = 2; // Slope

    std::vector<int> rainbow = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_BLUE, COLOR_MAGENTA};
    for (std::size_t i = 2; i < rainbow.size() + 2; i++) {
        init_pair(i, rainbow[i - 2], COLOR_BLACK);
    }
    int color = 0;
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
                        attron(COLOR_PAIR(color + 2));
                    } else {
                        attron(COLOR_PAIR(PURPLE_PAIR));
                    }
                } break;
                case 119: { // w
                    attron(COLOR_PAIR(PURPLE_PAIR));
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
        struct pos new_dir = logo.collision(max_size, dir);
        if (gay && (new_dir.x * dir.x == -1 || new_dir.y * dir.y == -1)) {
            color = (color + 1) % rainbow.size();
            attron(COLOR_PAIR(color + 2));
        }
        dir = new_dir;
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
    Config conf;
    if (!conf.hasLoaded()) {
        std::cout << "Config File Failed to load..." << std::endl;
        return 1;
    }
    conf.logo_path = file_path;

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
    std::vector<std::string> logo = getLogoFromFile(conf.logo_path.data(), &logo_len, &logo_height, &file_succ);
    if (!file_succ) {
        endwin();
        printf("File %s couldn't be opened.\n", file_path);
        return 1;
    }

    Logo logo_obj(0, 0, logo_height, logo_len, logo);
    mainLoop(logo_obj, conf);
    endwin();
    printf("Exiting gracefully...\n");
    return 0;
}
