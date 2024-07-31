#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 80000
#define TIMEOUT 100

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} direction_type;

typedef struct point {
    int x;
    int y;
} point;

point trophies[1]; // Only one trophy at a time
int trophyValue = 0; // Value of the current trophy
time_t trophyExpiration = 0;

int x = 0, y = 0, maxY = 0, maxX = 0, nextX = 0, nextY = 0, tailLength = 3;
bool gameOver = false;
direction_type currentDir = RIGHT;
point snakeParts[255] = {0};

void drawPart(point drawPoint) {
    mvprintw(drawPoint.y, drawPoint.x, "o");
}

void cursesInit() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    timeout(TIMEOUT);
    curs_set(FALSE);
    getmaxyx(stdscr, maxY, maxX);
}

void init() {
    srand(time(NULL));
    currentDir = RIGHT;
    tailLength = 3;
    gameOver = false;
    clear();

    int j = 0;
    for (int i = tailLength; i >= 0; i--) {
        point currPoint;
        currPoint.x = i;
        currPoint.y = maxY / 2;
        snakeParts[j] = currPoint;
        j++;
    }

    spawnTrophy();
    refresh();
}

void spawnTrophy() {
    point newTrophy;
    newTrophy.x = rand() % (maxX - 2) + 1;
    newTrophy.y = rand() % (maxY - 2) + 1;

    bool onSnake = false;
    for (int i = 0; i < tailLength; i++) {
        if (snakeParts[i].x == newTrophy.x && snakeParts[i].y == newTrophy.y) {
            onSnake = true;
            break;
        }
    }

    if (!onSnake) {
        trophies[0] = newTrophy;
        trophyValue = rand() % 9 + 1;
        trophyExpiration = time(NULL) + rand() % 9 + 1;
    } else {
        spawnTrophy(); // Try to spawn again if it overlaps with the snake
    }
}

void drawTrophy() {
    mvprintw(trophies[0].y, trophies[0].x, "%d", trophyValue);
}

void shiftSnake() {
    point tmp = snakeParts[tailLength - 1];
    for (int i = tailLength - 1; i > 0; i--) {
        snakeParts[i] = snakeParts[i - 1];
    }
    snakeParts[0] = tmp;
}

void drawScreen() {
    clear();

    for (int i = 0; i < maxY; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, maxX - 1, "#");
    }
    for (int i = 0; i < maxX; i++) {
        mvprintw(0, i, "#");
        mvprintw(maxY - 1, i, "#");
    }

    if (gameOver)
        mvprintw(maxY / 2, maxX / 2 - 5, "Game Over!");

    for (int i = 0; i < tailLength; i++) {
        drawPart(snakeParts[i]);
    }

    drawTrophy();
    refresh();
    usleep(DELAY);
}

int main(int argc, char *argv[]) {
    cursesInit();
    init();

    int ch;
    while (1) {
        getmaxyx(stdscr, maxY, maxX);
        if (gameOver) {
            sleep(2);
            init();
        }

        ch = getch();
        if ((ch == 'l' || ch == 'L' || ch == KEY_RIGHT) && (currentDir != RIGHT && currentDir != LEFT)) {
            currentDir = RIGHT;
        } else if ((ch == 'h' || ch == 'H' || ch == KEY_LEFT) && (currentDir != RIGHT && currentDir != LEFT)) {
            currentDir = LEFT;
        } else if ((ch == 'j' || ch == 'J' || ch == KEY_DOWN) && (currentDir != UP && currentDir != DOWN)) {
            currentDir = DOWN;
        } else if ((ch == 'k' || ch == 'K' || ch == KEY_UP) && (currentDir != UP && currentDir != DOWN)) {
            currentDir = UP;
        }

        if (((ch == 'l' || ch == 'L' || ch == KEY_RIGHT) && (currentDir == LEFT)) ||
            ((ch == 'h' || ch == 'H' || ch == KEY_LEFT) && (currentDir == RIGHT)) ||
            ((ch == 'j' || ch == 'J' || ch == KEY_DOWN) && (currentDir == UP)) ||
            ((ch == 'k' || ch == 'K' || ch == KEY_UP) && (currentDir == DOWN))) {
            gameOver = true;
        }

        nextX = snakeParts[0].x;
        nextY = snakeParts[0].y;

        if (currentDir == RIGHT)
            nextX++;
        else if (currentDir == LEFT)
            nextX--;
        else if (currentDir == UP)
            nextY--;
        else if (currentDir == DOWN)
            nextY++;

        if ((nextX >= maxX - 1 || nextX <= 0) || (nextY >= maxY - 1 || nextY <= 0)) {
            gameOver = true;
        }

        shiftSnake();
        snakeParts[0].x = nextX;
        snakeParts[0].y = nextY;

        for (int i = 1; i < tailLength; i++) {
            if (nextX == snakeParts[i].x && nextY == snakeParts[i].y) {
                gameOver = true;
                break;
            }
        }

        if (snakeParts[0].x == trophies[0].x && snakeParts[0].y == trophies[0].y) {
            tailLength += trophyValue;
            spawnTrophy();
        }

        if (time(NULL) >= trophyExpiration) {
            spawnTrophy();
        }

        drawScreen();
    }
    endwin();
    nocbreak();
    return 0;
}