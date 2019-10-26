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
    PID1 = Buyer
    PID2 = Seller
    PID3 = Market
    */
    char buff[96]; 
    int status, pid1, pid2, pid3; 
}; 
  
struct memory* shmptr; 
  
void handler(int signum) {  
    if (signum == SIGUSR1) {
    }
    else if (signum == SIGUSR2) {
        printf("\nCounteroffer: "); 
        puts(shmptr->buff); 

        sleep(1);   
        printf("Offer: "); 
        fgets(shmptr->buff, 96, stdin); 
        shmptr->status = Ready; 
        kill(shmptr->pid2, SIGUSR2); 
    }
} 

void createSeller() {
	printf("Waiting for seller...\n"); 
    printf("Seller connected.\n");
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
    shmptr->status = Ready;     
    signal(SIGUSR1, handler); 
    signal(SIGUSR2, handler); 
    strcpy(shmptr->buff, connectionConfirmed(buyer_ID));
    kill(shmptr->pid3, SIGUSR1); 
    kill(shmptr->pid2, SIGUSR1); 

    sleep(1);   
    printf("\nOffer: "); 
    fgets(shmptr->buff, 96, stdin); 
    shmptr->status = Ready; 
    kill(shmptr->pid2, SIGUSR2); 

    while (1) {      
  
        while (shmptr->status == Ready) 
            continue; 

    } 
    
    shmdt((void*)shmptr); 

    return 0;

}