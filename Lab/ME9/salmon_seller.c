#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void createSeller(char argv[]) {
	printf("Waiting for buyer...\n"); 
	// printf("ID: %s\n", argv);
}

void checkArgs(int argc, char argv[]) { 
	argc == 2 ? 
		createSeller(argv) : 
			(printf("Error, no argument for Seller ID. Exiting...\n"), exit(0));
}

char* connectionConfirmed(char argv[]) {
	static char toSend[50];
	strcpy(toSend, "Item ");
	strcat(toSend, argv);
	strcat(toSend, ": Seller connected.");
	// printf("%s\n", toSend);
	return toSend;
}

int main(int argc, char **argv) {

	char* seller_ID = argv[1];

	checkArgs(argc, seller_ID);
	// printf("%s\n", connectionConfirmed(seller_ID));

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, 1024, 0666|IPC_CREAT); 
    char *connectionMessage = (char*) shmat(shmid,(void*) 0, 0); 
    strcpy(connectionMessage, connectionConfirmed(seller_ID));

    // printf("Data written in memory: %s\n",connectionMessage); 
    
    //detach from shared memory  
    shmdt(str); 

    shmctl(shmid, IPC_RMID, NULL); 

    return 0;

}