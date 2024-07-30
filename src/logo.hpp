#ifndef LOGO_H
#define LOGO_H
#include <string>
#include <vector>
#include <stdbool.h>

std::vector<std::string> getLogoFromFile(char *filename, int *logo_len, int *num_lines, bool *exit_code);

void printLogo(std::vector<std::string>logo, int y, int x, int logo_len, int logo_height);

void clearLogo(int y, int x, int logo_len, int logo_height);


#endif
