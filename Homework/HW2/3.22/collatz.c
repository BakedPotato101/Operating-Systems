/*
 * Author: Jackson Baker
 * Date: 2/10/2025
 * Description: This program takes a positive integer n as input and uses a
 * child process to write into a shared memory space the calculations of the
 * collatz algorithm. The parent then reads the shared memory space and prints
 * to the standard out.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int numlen(long long num) {
  int digits = 1;
  if (num < 0) {
    return -1;
  }
  while (num >= 10) {
    num /= 10;
    digits++;
  }
  return digits;
}

int collatz(int n, char **ptr) {
  if (n == 1) {
    sprintf(*ptr, "%d \n", n);
    *ptr += strlen(" \n") + numlen(n);
    return 0;
  } else if ((n % 2) == 0) { // even
    sprintf(*ptr, "%d ", n);
    *ptr += numlen(n) + 1;
    collatz(n / 2, ptr);
    return 0;
  } else { // odd
    sprintf(*ptr, "%d ", n);
    *ptr += numlen(n) + 1;
    int N = 3 * n + 1;

    if (N <= 0) { // error check
      char *m =
          "\n 3n+1 overflowed to a negative integer. Try a smaller number.";
      sprintf(*ptr, "%s", m);
      *ptr += strlen(m);
      return 1;
    }
    collatz(N, ptr);
    return 0;
  }
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

  const int SIZE = 4096;
  const char *NAME = "jab132_collatz";
  int shm_fd;

  if (fork() == 0) { // child code
    void *sharedMemPtrW;
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    sharedMemPtrW = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    collatz(n, (char **)&sharedMemPtrW);
    msync(sharedMemPtrW, SIZE, MS_SYNC);

    munmap(sharedMemPtrW, SIZE);
    close(shm_fd);
  } else { // parent code
    wait(NULL);
    shm_fd = shm_open(NAME, O_RDONLY, 0666);
    void *sharedMemPtrR = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("%s", (char *)sharedMemPtrR);

    munmap(sharedMemPtrR, SIZE);
    close(shm_fd);
    shm_unlink(NAME);
  }

  return 0;
}
