#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


int comparefunc(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int revcomparefunc(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
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
    char *files[100]; 
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

    for (int i = 0; i < num_files; i++) {
        printf("%s\n", files[i]);
        free(files[i]);
    }

    return 0;
}


