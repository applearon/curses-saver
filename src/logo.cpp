#include "logo.hpp"
#include <fstream>
#include <ncurses.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <vector>


std::vector<std::string> getLogoFromFile(char *filename, int *logo_len, int *num_lines, bool *exit_code) { // logo_len -> width; // num_lines -> height
    std::string out;
    std::ifstream file;

    file.open(filename);
    if (file.fail()) {
        printf("Failed to open file\n");
        *exit_code = false;
        std::vector<std::string> out;
        return out;
    }
    *logo_len = 0;
    *num_lines = 0;
    std::string line;
    std::vector<std::string> output;
    while (std::getline(file, line)) {
        *logo_len = std::max<int>(line.length(), *logo_len);
        ++*num_lines;
        output.resize(*num_lines);
        output[*num_lines-1] = line;
    }
    *exit_code = true;
    file.close();
    return output;
}

void printLogo(std::vector<std::string> logo, int y, int x, int logo_len, int logo_height) {
    move(y, x);
    attron(COLOR_PAIR(1));
    //char *temp = malloc(sizeof(char) * logo_len);
    for (int i = 0; i < logo_height; ++i) {
        //strncpy(temp, logo + (i * logo_len), logo_len);
        move(y + i, x);
        printw("%s", logo[i].data());
    }
    attroff(COLOR_PAIR(1));
    //free(temp);
}


void clearLogo(int y, int x, int logo_len, int logo_height) {
    std::string tmp = "";
    for (int i = 0; i <= logo_len; ++i) {
        tmp.append(" ");
    }
    for (int i = 0; i < logo_height; ++i) {
        move(y + i, x);
        printw("%s", tmp.data());
    }
}
