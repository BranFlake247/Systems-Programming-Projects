#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 1024

// these flags match the four options used in cat lab
int flagB=0, flagE=0, flagN=0, flagS=0;

void processFile(const char *filename) {
    int src;
    char buffer[BUFFERSIZE];
    int numBytes;
    int lineNumber = 1;
    int isNonBlankLine = 0;

    src = open(filename, O_RDONLY); // Attempt to open the source file
    if(src < 0) {
        perror(filename);
        return;
    }

    while ((numBytes = read(src, buffer, BUFFERSIZE)) > 0) {
        for (int i = 0; i < numBytes; i++) {
            if (flagN && !flagB) {
                if (buffer[i] == '\n') {
                    char lineNumberStr[16];
                    int len = snprintf(lineNumberStr, sizeof(lineNumberStr), "%6d\t", lineNumber++);
                    write(STDOUT_FILENO, lineNumberStr, len);
                }
            }

            if (flagB && !flagN) {
                if (buffer[i] != '\n' && buffer[i] != ' ' && buffer[i] != '\t') {
                    if (!isNonBlankLine) {
                        char lineNumberStr[16];
                        int len = snprintf(lineNumberStr, sizeof(lineNumberStr), "%6d\t", lineNumber++);
                        write(STDOUT_FILENO, lineNumberStr, len);
                        isNonBlankLine = 1;
                    }
                }
            }

            if (flagN && flagB) {
                if (buffer[i] == '\n') {
                    char lineNumberStr[16];
                    int len = snprintf(lineNumberStr, sizeof(lineNumberStr), "%6d\t", lineNumber++);
                    write(STDOUT_FILENO, lineNumberStr, len);
                }
            }


            if (flagE) {
                if (buffer[i] == '\n') {
                    write(STDOUT_FILENO, "$", 1); 
                }
            }

            write(STDOUT_FILENO, &buffer[i], 1);

            if (buffer[i] == '\n') {
                isNonBlankLine = 0;
            }
        }
    }

    // Handle read errors
    if(numBytes < 0) {
        perror("Read error");
        close(src);
        return;
    }

    // Attempt to close the source file
    if(close(src) < 0) {
        perror("Error closing file");
        return;
    }
}



int main(int ac, char *av[]) {
    int option;

    // Parse options
    while((option = getopt(ac, av, "bens")) != -1) {
        switch(option) {
            case 'b':   // Number the non-blank output lines, starting at 1
                flagB = 1;
                printf("flag B enabled\n");
                break;
            case 'e':   // Display a dollar sign ('$') at the end of each line
                flagE = 1;
                printf("flag E enabled\n");
                break;
            case 'n':   // Number the output lines, starting at 1
                flagN = 1;
                printf("flag N enabled\n");
                break;
            case 's':   // Squeeze (remove) all empty lines, causing the output to be single spaced
                flagS = 1;
                printf("flag S enabled\n");
                break;  
            default:
                printf("Invalid option\n");
                return 1;
        }
    }

    // Process each file provided as argument
    for(int i = optind; i < ac; i++) {
        printf("Processing file: %s\n", av[i]);
        processFile(av[i]);
    }

    return 0;
}