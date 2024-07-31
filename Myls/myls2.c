#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_FILES 1000
#define COLUMN_WIDTH 20

int comparefunc(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int revcomparefunc(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
}

void print_files_in_columns(char *files[], int num_files, int column_width) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = w.ws_col;
    int num_columns = terminal_width / column_width;

    for (int i = 0; i < num_files; i++) {
        printf("%-*s", column_width, files[i]);
        if ((i + 1) % num_columns == 0) {
            printf("\n");
        }
    }
    if (num_files % num_columns != 0) {
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int list_hidden = 0;     
    int sort = 0;            
    int reverse_sort = 0;    
    char *dir_name = ".";    

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] == 'a') {
                    list_hidden = 1;
                } else if (argv[i][j] == 's') {
                    sort = 1;
                } else if (argv[i][j] == 'r') {
                    reverse_sort = 1;
                    sort = 1; 
                }
            }
        } else {
            dir_name = argv[i];
        }
    }

    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");   
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char *files[MAX_FILES]; 
    int num_files = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!list_hidden && entry->d_name[0] == '.')
            continue;
        files[num_files] = strdup(entry->d_name);
        num_files++;
    }

    closedir(dir); 

    if (sort) {
        if (reverse_sort) {
            qsort(files, num_files, sizeof(char *), revcomparefunc);
        } else {
            qsort(files, num_files, sizeof(char *), comparefunc);
        }
    }

    print_files_in_columns(files, num_files, COLUMN_WIDTH);

    for (int i = 0; i < num_files; i++) {
        free(files[i]);
    }

    return 0;
}

