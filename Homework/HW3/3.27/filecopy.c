#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1


int main(int argc, char* argv[]) {
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0) { // Parent
        close(fd[READ_END]);
        FILE *fp = fopen(argv[1], "r");
        char c;
        while ((c = fgetc(fp)) != EOF) {
            write(fd[WRITE_END], &c, sizeof(c));
        }
        fclose(fp);
        close(fd[WRITE_END]);
        wait(NULL);
    } else { // Child
        close(fd[WRITE_END]);
        FILE *fp = fopen(argv[2], "w");
        char c;
        while (read(fd[READ_END], &c, sizeof(c)) > 0) {
            fputc(c, fp);
        }
        fclose(fp);
        close(fd[READ_END]);
    }

}