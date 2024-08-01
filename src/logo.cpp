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

Logo::Logo(int y, int x, int height, int length, std::vector<std::string> &logo) {
    this->position = {y, x};
    this->size = {height, length};
    this->logo = logo;
};

bool Logo::print() {
    ::move(position.y, position.x);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < size.y; ++i) {
        ::move(position.y + i, position.x);
        printw("%s", logo[i].data());
    }
    attroff(COLOR_PAIR(1));
    return true;
};

bool Logo::clear() {
            std::string tmp(size.x, ' ');
            for (int i = 0; i < size.y; ++i) {
                ::move(position.y + i, position.x);
                printw("%s", tmp.data());
            }
            return true;
}

bool Logo::move(int y, int x, const pos &min_size, const pos &max_size) {
    clear();
    this->position.y += y;
    this->position.x += x;
    print();
    return true;
}

bool Logo::put(int y, int x, const pos &min_size, const pos &max_size) {
    clear();
    this->position.y = y;
    this->position.x = x;
    print();
    return true;
}

const pos Logo::getPos() {
    return this->position;
}
const pos Logo::getSize() {
    return this->size;
}
