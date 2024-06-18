#ifndef LOGO_H
#define LOGO_H
#include <stdbool.h>

void getLogo(char *logo, int slope, int *logo_len, int *logo_height);

char **getLogoFromFile(char *filename, int *logo_len, int*logo_height, bool *exit_code);

void printLogo(char **logo, int y, int x, int logo_len, int logo_height);

void clearLogo(int y, int x, int logo_len, int logo_height);


#endif
