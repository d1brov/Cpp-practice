#include <stdio.h>
#include <stdlib.h>

#include "effective_copy.h"

#define MAX_BUF     1024 * 1024
#define DEFAULT_BUF 1024

int main(int argc, char **argv) {

    if (argc < 3 || argc > 5) {
        printf(
            "Usage: %s <src file name> <dst file name> [buffer size bytes]\n",
            argv[0]);
        exit(EXIT_FAILURE);
    }

    int buffer_size = DEFAULT_BUF;

    // if buffer size argument was defined
    if (argc == 4) {
        if (sscanf(argv[3], "%d", &buffer_size) < 0) {
            perror("sscanf failed. Reason");
            exit(EXIT_FAILURE);
        }

        if (buffer_size > MAX_BUF) {
            fprintf(
                stderr, 
                "Buffer size cannot be larger than %d bytes\n", 
                MAX_BUF);
            exit(EXIT_FAILURE);
        }

        if(buffer_size < 0) {
            fprintf(
                stderr, 
                "Buffer size cannot be negative number\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return ecopy(argv[1], argv[2], buffer_size);
}