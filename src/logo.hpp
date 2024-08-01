#ifndef LOGO_H
#define LOGO_H
#include <string>
#include <vector>
#include <stdbool.h>

std::vector<std::string> getLogoFromFile(char *filename, int *logo_len, int *num_lines, bool *exit_code);

struct pos {
    int y;
    int x;
};


class Logo {

    public:
        struct pos pos;
        struct pos size;
        std::vector<std::string> logo;
        Logo(int y, int x, int height, int length, std::vector<std::string> &logo);
        bool print();
        bool clear();
    private:
};

#endif
