#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

void handle_signal(int sig) {
    endwin(); // End curses mode
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s speed\n", argv[0]);
        printf("Speed ranges from 1 to 99\n");
        return 1;
    }

    int speed = atoi(argv[1]);
    if (speed < 1 || speed > 99) {
        printf("Invalid speed. Speed must be between 1 and 99.\n");
        return 1;
    }

    signal(SIGINT, handle_signal);

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    srand(time(NULL));

    while (1) {
        move(0,0);
        insertln();
        for (int i = 0; i < cols; i++) {


                if (rand() % 4 == 0) {
                    if (rand() % 2 == 0) {
                        addch('1');
                    } else {
                        addch('0'); 
                    }
                } else {
                    addch(' '); 
                }
        }

        refresh();

        usleep(15000000 / speed);
    }
    endwin(); 
    return 0;
}
