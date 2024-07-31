#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

volatile sig_atomic_t largest_prime = 0;

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void handle_sigint(int sig) {
    printf("\n%d    Quit [y/n]? ", largest_prime);
    char choice;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    choice = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (choice == 'y' || choice == 'Y') {
        printf("\n");
        exit(0);
    }
    printf("\n");
}

int main() {
    signal(SIGINT, handle_sigint);
    
    int num = 2;
    while (1) {
        if (is_prime(num)) {
            largest_prime = num;
        }
        num++;
        sleep(1);  // Introduction a delay of 1 second to see the lower number results
    }
    
    return 0;
}
