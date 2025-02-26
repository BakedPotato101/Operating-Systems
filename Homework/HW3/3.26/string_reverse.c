#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1000

void reverse_string_case(char *str) {
  int i = 0;
  while (str[i] != '\0') {
    if (islower(str[i])) {
      str[i] = toupper(str[i]);
    } else if (isupper(str[i])) {
      str[i] = tolower(str[i]);
    }
    i++;
  }
  printf("String reversed...\n");
}

int main(int argc, char *argv[]) {

  char message[BUFFER_SIZE];
  int fd1[2];
  int fd2[2];
  pid_t pid;

  if (pipe(fd1) == -1) { // Parent -> Child
    fprintf(stderr, "Pipe failed");
    return 1;
  }
  if (pipe(fd2) == -1) { // Parent <- Child
    fprintf(stderr, "Pipe failed");
    return 1;
  }

  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed");
    return 1;
  }

  if (pid > 0) { // Parent
    
    close(fd1[READ_END]);
    printf("Enter a string: ");
    fgets(message, BUFFER_SIZE, stdin);

    printf("Parent writes: %s\n", message);
    write(fd1[WRITE_END], message, strlen(message) + 1);
    close(fd1[WRITE_END]);
    wait(NULL);

    close(fd2[WRITE_END]);
    read(fd2[READ_END], message, BUFFER_SIZE);
    printf("Parent Reads: %s\n", message);
    close(fd2[READ_END]);

  } else { // Child
    close(fd1[WRITE_END]);
    read(fd1[READ_END], message, BUFFER_SIZE);
    printf("Child Reads: %s\n", message);

    reverse_string_case(message);

    close(fd1[READ_END]);
    close(fd2[READ_END]);
    printf("Child writes: %s\n", message);
    write(fd2[WRITE_END], message, strlen(message) + 1);
    close(fd2[WRITE_END]);

  }
  return 0;
}