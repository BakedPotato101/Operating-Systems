/*
* Author: Jackson Baker
* Date: 2/10/2025
* Description: Zombie
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
printf("Parent Started\n");
pid_t id = fork();

if(id < -1) { //failed to fork
    perror("Fork Failed");
    return -1;
}

else if (id == 0) { //child executes
    printf("Child executed\n");
    return 0;
}

else { //parent executes
    printf("Parent executing\n");
    printf("PID = %i\n", id);
    printf("Sleeping for 10 seconds...\n");
    for(int i=0; i < 10; i++) {
        sleep(1);
    }
    printf("Parent Finished Executing.\n");
}
}