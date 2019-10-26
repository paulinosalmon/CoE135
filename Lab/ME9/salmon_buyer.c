#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
#define FILLED 0 
#define Ready 1 
#define NotReady -1 
  
struct memory { 
    /* 
    PID1 = Market
	PID2 = Seller
	PID3 = Buyer
    */
    char buff[96]; 
    int status, pid1, pid2, pid3; 
}; 
  
struct memory* shmptr; 
  
void handler(int signum) {  
    if (signum == SIGUSR1) {
        printf("Counteroffer of "); 
        puts(shmptr->buff); 
    }
} 

void createSeller() {
	printf("Waiting for seller...\n"); 
}

void checkArgs(int argc, char argv[]) { 
	argc == 2 ? 
		createSeller() : 
			(printf("Error, no argument for Buyer ID. Exiting...\n"), exit(0));
}

char* connectionConfirmed(char argv[]) {
	static char toSend[50];
	strcpy(toSend, "Item ");
	strcat(toSend, argv);
	strcat(toSend, ": Buyer connected.");
	return toSend;
}

int main(int argc, char **argv) {

	int pid = getpid();
	char* buyer_ID = argv[1];

	checkArgs(argc, buyer_ID);

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
    shmptr->pid1 = pid; 
    shmptr->status = NotReady; 

    // Initialization Message
    strcpy(shmptr->buff, connectionConfirmed(buyer_ID));
    shmptr->status = Ready;     
    signal(SIGUSR1, handler); 
    kill(shmptr->pid3, SIGUSR1); 

    while (1) { 
        sleep(1);   
        printf("Offer: "); 
        fgets(shmptr->buff, 100, stdin); 
        shmptr->status = Ready; 
  
        kill(shmptr->pid2, SIGUSR2); 
  
        while (shmptr->status == Ready) 
            continue; 
    } 
    
    shmdt((void*)shmptr); 

    return 0;

}