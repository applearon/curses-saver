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
#include "cmdargs.hpp"

#ifdef _WIN32
#include<windows.h>
#else
#include<unistd.h>
#endif

enum {
    PURPLE_PAIR = 1
};

void mainLoop(Logo &logo, Config &config) {
    // ncurses stuff
    initscr();
    curs_set(0);
    cbreak();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(PURPLE_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(PURPLE_PAIR));
    if (config.saver_mode) {
        mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
        printf("\033[?1003h");
    }

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
        if (config.saver_mode) { // see mouse movements
            printf("\033[?1003h");
        }
        timeout(0);
        input = getch();
        if (input != -1) {
            last_inp = input;
            if (config.saver_mode) {
                return;
            }
            switch (last_inp) {
                case 113: // q
                case 3: { // ctrl-c
                    move(0, 0);
                    return;
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
    return;
}

int main(int argc, char *argv[0]) {
    Config conf;
    Arguments args(argc, argv);
    if (args.Failed()) {
        std::cout << "Arguments failed to load..." << std::endl;
        return 1;
    }
    if (args.config_path != "") {
        conf = Config(args.config_path);
    }
    if (!conf.hasLoaded()) {
        std::cout << "Config File " << args.config_path << " Failed to load..." << std::endl;
        return 1;
    }
    if (args.logo_path != "") {
        conf.logo_path = args.logo_path;
    }
    initscr();
    if (has_colors() == false) {
        endwin();
        std::cout << "This terminal doesn't have colour support." << std::endl;
        return 1;
    }
    endwin();
    bool file_succ;
    int logo_len = 0;
    int logo_height = 0;
    std::vector<std::string> logo = getLogoFromFile(conf.logo_path.data(), &logo_len, &logo_height, &file_succ);
    if (!file_succ) {
        endwin();
        std::cout << "File" << conf.logo_path << " couldn't be opened." << std::endl;
        return 1;
    }
    Logo logo_obj(0, 0, logo_height, logo_len, logo);
    mainLoop(logo_obj, conf);
    printf("\033[?1003l");
    endwin();
    printf("Exiting gracefully...\n");
    return 0;
}
