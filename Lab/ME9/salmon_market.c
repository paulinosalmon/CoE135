#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main() {

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, 1024, 0666|IPC_CREAT); 

    printf("Market pool: 0\n");
    printf("Waiting for connections...\n");

    while(1) {
		char *connectionNotification = (char*) shmat(shmid,(void*) 0, 0); 
		printf("%s\n", connectionNotification); 
	}





    return 0;

}