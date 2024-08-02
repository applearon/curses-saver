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
        Logo(int y, int x, int height, int length, std::vector<std::string> &logo);
        bool move(int y, int x, const pos &min_size, const pos &max_size);
        bool put(int y, int x, const pos &min_size, const pos &max_size);
        struct pos collision(const struct pos &max_size, const struct pos &old_dir);

        const pos getPos();
        const pos getSize();
    private:
        bool print();
        bool clear();
        pos position;
        pos size;
        std::vector<std::string> logo;
};

#endif
