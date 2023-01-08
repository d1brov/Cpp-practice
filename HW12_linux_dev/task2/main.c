#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64

int child_logic(int writing_fd) {
    const char* child_message = "Hi daddy, I'm your child!";

    if(write(writing_fd, child_message, strlen(child_message)) < 0) {
        perror("child writing to FD error. Reason");
        return -1;
    }

    return 0;
}

int parrent_logic(int reading_fd) {

    // opening file
    FILE* dialog_file = fopen("dialog.txt", "w");
    if(!dialog_file) {
        perror("File cannot be opened. Reason");
        return -1;
    }

    char buf[BUF_SIZE];
    int len;
    do {
        // reading message from pipe
        len = read(reading_fd, buf, BUF_SIZE);
        if(len < 0) {
            perror("parent reading from FD error. Reason");
            break;
        }

        // writing message to file
        if(fwrite(buf, 1, len, dialog_file) < 0) {
            perror("Error writing to file. Reason");
            break;
        }
    }
    while (len);

    // append "Hi"
    if(fputs("\nHi!\n", dialog_file) < 0) {
        perror("Error writing to file. Reason");
    }

    if(fclose(dialog_file) ) {
        perror("File cannot be closed. Reason");
        return -1;
    }
    
    return 0;
}

int main(int argc, char **argv) {

    int pipefds[2];
    pipe(pipefds);

    pid_t child_pid = fork();
    if(child_pid < 0) {
        perror("fork() failed. Reason");
        return -1;
    }

     if(child_pid) {
        // parent process
        if(close(pipefds[1]) < 0) {
            perror("parent closing writing FD error. Reason");
            return -1;
        }

        parrent_logic(pipefds[0]);

        if(close(pipefds[0]) < 0) {
            perror("parent closing reading FD error. Reason");
            return -1;
        }
     }
     else {
        // child process
        if(close(pipefds[0]) < 0) {
            perror("child closing reading FD error. Reason");
            return -1;
        }

        child_logic(pipefds[1]);

        if(close(pipefds[1]) < 0) {
            perror("child closing writing FD error. Reason");
            return -1;
        }
        return 0;
     }
}