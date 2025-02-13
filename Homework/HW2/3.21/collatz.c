/*
 * Author: Jackson Baker
 * Date: 2/10/2025
 * Description: This program takes a positive integer n as input and uses a
 * child process to print to the screen the results of performing the collatz
 * algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int collatz(int n) {
    if(n == 1) {
        printf("%d \n", n);
        return 0;
    }
    else if((n % 2) == 0) {
        printf("%d ", n);
        collatz(n/2);
    }
    else {
        printf("%d ", n);
        if ((3*n+1) < 0) {
            printf("3n+1 overflowed... choose a new number");
            return 1;
        }
        collatz(3*n+1);
    }
    return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) { // Error: incorrect usage
    printf("Usage: %s <positive integer>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  if (n <= 0) { // Error: negative number.
    printf("n must be greater than zero.\n");
    return 1;
  }

  if (fork() == 0) { // child code
    collatz(n);

  } else { // parent code
    wait(NULL);
  }

  return 0;
}
