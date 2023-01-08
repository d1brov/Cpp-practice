#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include "effective_copy.h"

int ecopy(const char* src_fname, const char* dst_fname, unsigned int buf_size) {

    int src_fd = open(src_fname, O_RDONLY);
    if (src_fd < 0) {
        perror("Source file open failed. Reason");
        exit(EXIT_FAILURE);
    }

    struct stat stat_buf;
    if (fstat(src_fd, &stat_buf) < 0) {
        perror("fstat failed. Reason");
        exit(EXIT_FAILURE);
    }

    int dst_fd = open(dst_fname, O_WRONLY | O_CREAT, stat_buf.st_mode);
    if (dst_fd < 0) {
        perror("Destination file creation failed. Reason");
        exit(EXIT_FAILURE);
    }

    int n = 0;
    do {
        n = sendfile(dst_fd, src_fd, 0, buf_size);
        if (n < 0) {
            perror("sendfile failed. Reason");
            exit(EXIT_FAILURE);
        }
    } 
    while(n > 0);

    return EXIT_SUCCESS;
}