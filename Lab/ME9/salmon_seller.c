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
char seller_ID[50], price[100];
char priceComp1[25], priceComp2[25];

char *choppy(char *s) {
    char *n = malloc(strlen(s ? s: "\n"));
    if(s)
        strcpy(n, s);
    n[strlen(n) - 1] = '\0';
    return n;
}

void priceCompare(char *first, char *second) {
    int finalPrice = 0;
    int commission = 0;
    char *pointer;

    if(!strcmp(first, second)) {
        finalPrice = strtol(first, &pointer, 10);
        commission = finalPrice*0.10;
        finalPrice = finalPrice - commission;
        printf("\nYou’ve now sold your item. You received %d, %d goes to the market.\n", finalPrice, commission);
        kill(shmptr->pid1, SIGTERM);

        strcpy(shmptr->buff, "Item ");
        strcat(shmptr->buff, seller_ID);
        strcat(shmptr->buff, " Sold.");
        shmptr->commission = commission;

        kill(shmptr->pid3, SIGUSR2);
        exit(0);
    }
}

void handler(int signum) {
    if (signum == SIGUSR1) {
        // add start flag to negate this if sigusr1 is needed again
        printf("Buyer connected.\n");
    } 
    // Counter offering communication
    else if (signum == SIGUSR2) {
        printf("\nOffer of "); 
        puts(shmptr->buff); 
        strcpy(priceComp2, choppy(shmptr->buff));


        printf("Counteroffer: "); 
        // sleep(1); 
        fgets(shmptr->buff, 96, stdin); 
        strcpy(price, choppy(shmptr->buff));
        strcpy(priceComp1, price);


        shmptr->status = FILLED; 
        kill(shmptr->pid1, SIGUSR2); 

        sleep(1);
        strcpy(shmptr->buff, "Item ");
        strcat(shmptr->buff, seller_ID);
        strcat(shmptr->buff, ": Counteroffer ");
        strcat(shmptr->buff, price);
        kill(shmptr->pid3, SIGUSR1); 

        priceCompare(priceComp1, priceComp2);

    }
} 

void createSeller() {
	printf("Waiting for buyer...\n"); 
}

void checkArgs(int argc, char argv[]) { 
	argc == 2 ? 
		createSeller() : 
			(printf("Error, no argument for Seller ID. Exiting...\n"), exit(0));
}

char* connectionConfirmed(char argv[]) {
	static char toSend[50];
	strcpy(toSend, "Item ");
	strcat(toSend, argv);
	strcat(toSend, ": Seller connected.");
	return toSend;
}

int main(int argc, char **argv) {

	int pid = getpid(); 
    checkArgs(argc, seller_ID);
    strcpy(seller_ID , argv[1]);
    char* price[100];

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
    shmptr->pid2 = pid; 
    shmptr->status = NotReady; 

    // Initialization Message
    shmptr->status = Ready;   
    signal(SIGUSR1, handler);   
    signal(SIGUSR2, handler); 
    strcpy(shmptr->buff, connectionConfirmed(seller_ID));
    kill(shmptr->pid3, SIGUSR1); 

    while (1) { 

        while (shmptr->status != Ready) 
            continue; 

    } 

    shmdt((void*)shmptr); 

    return 0;

}