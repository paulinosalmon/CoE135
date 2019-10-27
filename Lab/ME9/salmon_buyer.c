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
    char buff[92]; 
    int status, pid1, pid2, pid3; 
    int commission;
}; 
  
struct memory* shmptr; 
char buyer_ID[50], price[100];
char priceComp1[25], priceComp2[25];

char *choppy(char *s) {
    char *n = malloc(strlen(s ? s: "\n"));
    if(s)
        strcpy(n, s);
    n[strlen(n) - 1] = '\0';
    return n;
}

void priceCompare(char *first, char *second) {
    if(!strcmp(first, second)) {
        printf("You've now bought Item %s.\n", buyer_ID);
        exit(0);
    }
}
  
void handler(int signum) {  
    if (signum == SIGUSR1) {
    }
    // Counter offering communication
    else if (signum == SIGUSR2) {
        printf("\nCounteroffer of "); 
        puts(shmptr->buff); 
        strcpy(priceComp2, choppy(shmptr->buff));

        priceCompare(priceComp1, priceComp2);

        // sleep(1);   
        printf("Offer: "); 
        fgets(shmptr->buff, 96, stdin); 
        strcpy(price, choppy(shmptr->buff));
        shmptr->status = Ready; 
        kill(shmptr->pid2, SIGUSR2); 
        sleep(1);
        strcpy(shmptr->buff, "Item ");
        strcat(shmptr->buff, buyer_ID);
        strcat(shmptr->buff, ": Offer ");
        strcat(shmptr->buff, price);
        kill(shmptr->pid3, SIGUSR1); 
    }
    else if (signum == SIGTERM) {
        printf("\nYou've now bought Item %s.\n", buyer_ID);
        exit(0);
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

    checkArgs(argc, buyer_ID);

	int pid = getpid();
	strcpy(buyer_ID, argv[1]);

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
    shmptr->pid1 = pid; 
    shmptr->status = NotReady; 

    // Initialization Message
    shmptr->status = Ready;     
    signal(SIGUSR1, handler); 
    signal(SIGUSR2, handler); 
    signal(SIGTERM, handler);
    strcpy(shmptr->buff, connectionConfirmed(buyer_ID));
    kill(shmptr->pid3, SIGUSR1); 
    kill(shmptr->pid2, SIGUSR1); 

    // sleep(1);   
    printf("\nOffer: "); 
    fgets(shmptr->buff, 96, stdin); 
    strcpy(price, choppy(shmptr->buff));
    strcpy(priceComp1, price);
    shmptr->status = Ready; 
    kill(shmptr->pid2, SIGUSR2); 

    sleep(1);
    strcpy(shmptr->buff, "Item ");
    strcat(shmptr->buff, buyer_ID);
    strcat(shmptr->buff, ": Offer ");
    strcat(shmptr->buff, price);
    kill(shmptr->pid3, SIGUSR1); 

    while (1) {      
  
        while (shmptr->status == Ready) 
            continue; 

    } 
    
    shmdt((void*)shmptr); 

    return 0;

}