#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_LINE_LENGTH 1024
#define GREETING_MESSAGE "Message from another terminal...\n"
#define EOF_MESSAGE "EOF\n"


int contains_prohibited_word(char *line) {
    char *keywords[] = {"apple", "pear", "banana", "plum"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; ++i) {
        if (strstr(line, keywords[i]) != NULL) {
            fprintf(stderr, "You entered a prohibited word: %s. Your message will not be sent.\n", keywords[i]);
            return 1; // Found prohibited word
        }
    }
    return 0; 
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <terminal>\n", argv[0]);
        return 1;
    }

    // Opens the termal for writing
    char *terminal = argv[1];
    int fd = open(terminal, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open terminal file");
        return 1;
    }

    // Send greeting message to the terminal
    if (write(fd, GREETING_MESSAGE, strlen(GREETING_MESSAGE)) == -1) {
        perror("Failed to write greeting message");
        close(fd);
        return 1;
    }

    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
        
        if (contains_prohibited_word(line)) {
            continue; // Skip sending this line
        }

        
        if (write(fd, line, strlen(line)) == -1) {
            perror("Failed to write line to terminal");
            close(fd);
            return 1;
        }
    }

    // Check if EOF or error occurred in stdin
    if (feof(stdin)) {
        // Send EOF message to the terminal
        if (write(fd, EOF_MESSAGE, strlen(EOF_MESSAGE)) == -1) {
            perror("Failed to write EOF message");
            close(fd);
            return 1;
        }
    } else if (ferror(stdin)) {
        perror("Error reading from stdin");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("Failed to close terminal file");
        return 1;
    }

    return 0;
}
