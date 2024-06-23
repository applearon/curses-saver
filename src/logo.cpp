#include "logo.hpp"
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUF_SIZE 65536

int count_lines(FILE* file) {
    char buf[BUF_SIZE];
    int counter = 0;
    for(;;) {
        size_t res = fread(buf, 1, BUF_SIZE, file);
        if (ferror(file)) {
            return -1;
        }

        int i;
        for(i = 0; i < res; i++) {
            if (buf[i] == '\n') {
                counter++;
            }
        }

        if (feof(file)) {
            break;
        }
    }

    return counter;
}

//const char **getLogoFromFile(char *filename, int *logo_len, int*logo_height, bool *exit_code) {
//    *logo_height = 0;
//    FILE *file = fopen(filename, "r");
//    struct stat path_stat;
//    stat(filename, &path_stat);
//    fseek(file, 0, SEEK_SET);
//    int total_lines = count_lines(file);
//    if (!S_ISREG(path_stat.st_mode) || total_lines == -1) {
//        *exit_code = false;
//        const char **out = malloc(sizeof(char *));
//        out[0] = NULL;
//        return out;
//    } 
//    fseek(file, 0, SEEK_END);
//    printf("Size of file: %lu\n", ftell(file));
//    const char **logo = malloc(sizeof(char *) * (total_lines));
//    fseek(file, 0, SEEK_SET);
//    ssize_t read;
//    size_t len = 0;
//    char *line = NULL;
//    while ((read = getline(&line, &len, file)) != -1) {
//        printf("Line len thing: %zu\n", len);
//        if (line[strlen(line) - 1] == '\n') {
//            line[strlen(line) -1] = '\0';
//        }
//        char *tmp = malloc((strlen(line) * 40) * sizeof(char));
//        strcpy(tmp, line);
//        *logo_len = strlen(line);
//        logo[*logo_height] = tmp;
//        //strcat(logo, line);
//        *logo_height += 1;
//    }
//    fclose(file);
//    *exit_code = true;
//    return logo;
//}

#define INITIAL_BUFFER_SIZE 1024
#define INITIAL_ARRAY_SIZE 10

int max(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

char **getLogoFromFile(char *filename, int *logo_len, int *num_lines, bool *exit_code) { //(char *filename, int *logo_len, int*logo_height, bool *exit_code)
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        *exit_code = false;
        return NULL;
    }

    char **lines = (char **) malloc(INITIAL_ARRAY_SIZE * sizeof(char *));
    if (!lines) {
        perror("Failed to allocate memory");
        fclose(file);
        *exit_code = false;
        return NULL;
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char *buffer = (char *) malloc(buffer_size);
    if (!buffer) {
        perror("Failed to allocate buffer memory");
        free(lines);
        fclose(file);
        *exit_code = false;
        return NULL;
    }

    size_t array_size = INITIAL_ARRAY_SIZE;
    *num_lines = 0;
    while (fgets(buffer, buffer_size, file)) {
        size_t len = strlen(buffer);

        // Handle lines longer than buffer_size
        while (len == buffer_size - 1 && buffer[len - 1] != '\n') {
            buffer_size *= 2;
            buffer = (char *) realloc(buffer, buffer_size);
            if (!buffer) {
                perror("Failed to reallocate buffer memory");
                for (int i = 0; i < *num_lines; ++i) {
                    free(lines[i]);
                }
                free(lines);
                fclose(file);
                *exit_code = false;
                return NULL;
            }
            fgets(buffer + len, buffer_size - len, file);
            len += strlen(buffer + len);
        }

        // Remove the newline character, if present
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        *logo_len = max(strlen(buffer) + 1, *logo_len);
        lines[*num_lines] = strdup(buffer);
        if (!lines[*num_lines]) {
            perror("Failed to allocate line memory");
            for (int i = 0; i < *num_lines; ++i) {
                free(lines[i]);
            }
            free(lines);
            free(buffer);
            fclose(file);
            *exit_code = false;
            return NULL;
        }

        (*num_lines)++;
        if (*num_lines >= array_size) {
            array_size *= 2;
            lines = (char **) realloc(lines, array_size * sizeof(char *));
            if (!lines) {
                perror("Failed to reallocate lines memory");
                for (int i = 0; i < *num_lines; ++i) {
                    free(lines[i]);
                }
                free(buffer);
                fclose(file);
                *exit_code = false;
                return NULL;
            }
        }
    }

    free(buffer);
    fclose(file);
    *exit_code = true;
    return lines;
}

void printLogo(char **logo, int y, int x, int logo_len, int logo_height) {
    move(y, x);
    attron(COLOR_PAIR(1));
    //char *temp = malloc(sizeof(char) * logo_len);
    for (int i = 0; i < logo_height; ++i) {
        //strncpy(temp, logo + (i * logo_len), logo_len);
        move(y + i, x);
        printw("%s", logo[i]);
    }
    attroff(COLOR_PAIR(1));
    //free(temp);
}

//void clearLogo(int y, int x, int logo_len, int logo_height) {
//
//};

void clearLogo(int y, int x, int logo_len, int logo_height) {
    char *temp = (char *) malloc(sizeof(char) * (logo_len + 1));
    //temp[0] = '\0';
    //strcpy(temp, "");
    for (int i = 0; i < logo_len; ++i) {
        //strcat(temp, " ");
        temp[i] = ' ';
    }
    temp[logo_len] = '\0';
    for (int i = 0; i < logo_height; ++i) {
        move(y + i, x);
        printw("%s", temp);
   }
    free(temp);
}
