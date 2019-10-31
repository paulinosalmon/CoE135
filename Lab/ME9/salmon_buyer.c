/*
Name: Paulino I. Salmon III
Section: HLMTRU

Base code reference: 
https://www.geeksforgeeks.org/chat-application-between-two-processes-using-signals-and-shared-memory/?fbclid=IwAR19l9dMPDJRTQbE8Df7MNepYF6Duj-3B9ufG4ow8T-Nbu1EXOrILwaibCE
*/

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
#define Default -1
  
struct memory { 
    /* 
    PID1 = Buyer
    PID2 = Seller
    PID3 = Market
    */
    char buff[44]; 
    int buyerFlag, sellerFlag;
    int ID1, ID2, ID4, ID5, ID6, ID7;
    int status, pid1, pid2, pid3, pid4, pid5, pid6, pid7; 
    int commission;
}; 
  
int target, revert;
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

void initialize() {

    if(shmptr->pid1 > 0 || shmptr->pid2 > 0 || shmptr->pid4 > 0 || shmptr->pid5 > 0 || shmptr->pid6 > 0 || shmptr->pid7 > 0) 
        return;

    else {
        shmptr->pid1 = Default;
        shmptr->pid2 = Default;
        shmptr->pid4 = Default;
        shmptr->pid5 = Default;
        shmptr->pid6 = Default;
        shmptr->pid7 = Default;

        shmptr->ID1 = Default;
        shmptr->ID2 = Default;
        shmptr->ID4 = Default;
        shmptr->ID5 = Default;
        shmptr->ID6 = Default;
        shmptr->ID7 = Default;
        return;
    }
}


void plugInID(int pid, char* buyer_ID_string) {
    long int buyer_ID_int;
    char *pointer;

    buyer_ID_int = strtol(buyer_ID_string, &pointer, 10);

    if(shmptr->ID2 == Default) {
        shmptr->pid2 = pid; 
        shmptr->ID2 = buyer_ID_int;
        revert = 2;
    }
    
    else if(shmptr->ID5 == Default) {
        shmptr->pid5 = pid; 
        shmptr->ID5 = buyer_ID_int;
        revert = 5;
    }

    else if(shmptr->ID7 == Default) {
        shmptr->pid7 = pid; 
        shmptr->ID7 = buyer_ID_int;
        revert = 7;
    }

    else {
        printf("Market is full. Cannot connect.\n");
        exit(0);
    }

}

int selectTarget(char* buyer_ID_string) {
    long int buyer_ID_int;
    char *pointer;

    buyer_ID_int = strtol(buyer_ID_string, &pointer, 10);

    if(buyer_ID_int == shmptr->ID1)
        return shmptr->pid1;
    else if (buyer_ID_int == shmptr->ID4)
        return shmptr->pid4;
    else if (buyer_ID_int == shmptr->ID6)
        return shmptr->pid6;    else {
        printf("Market is full. Cannot connect.\n");
        exit(0);
    }

}

void revertID() {
    if(revert == 2)
        shmptr->ID2 = Default;
    else if(revert == 5)
        shmptr->ID5 = Default;
    else if(revert == 7)
        shmptr->ID7 = Default;
    return;
}

void priceCompare(char *first, char *second) {
    if(!strcmp(first, second)) {
        printf("You've now bought Item %s.\n", buyer_ID);
        revertID();
        shmptr->buyerFlag = 0;
        exit(0);
    }
}
  
void handler(int signum) {  

    if (signum == SIGUSR1) {
        printf("Seller connected.\n");
        target = selectTarget(buyer_ID);
    }

    // Counter offering communication
    else if (signum == SIGUSR2) {
        printf("\nCounteroffer of "); 
        puts(shmptr->buff); 
        strcpy(priceComp2, choppy(shmptr->buff));

        priceCompare(priceComp1, priceComp2);

        printf("Offer: "); 
        fgets(shmptr->buff, 96, stdin); 
        strcpy(price, choppy(shmptr->buff));
        shmptr->status = Ready; 
        kill(target, SIGUSR2); 
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
    shmptr->buyerFlag = 1;
    initialize();

    printf("Waiting for seller...\n"); 
    shmptr->status = Ready;     
    signal(SIGUSR1, handler); 
    signal(SIGUSR2, handler); 
    signal(SIGTERM, handler);
    strcpy(shmptr->buff, connectionConfirmed(buyer_ID));
    kill(shmptr->pid3, SIGUSR1); 

    while(shmptr->sellerFlag == 0) {

    }
    printf("Seller connected.");

    plugInID(pid, buyer_ID);
    target = selectTarget(buyer_ID);
    shmptr->status = NotReady; 

    // Initialization Message
    kill(target, SIGUSR1); 

    printf("\nOffer: "); 
    fgets(shmptr->buff, 96, stdin); 
    strcpy(price, choppy(shmptr->buff));
    strcpy(priceComp1, price);
    shmptr->status = Ready; 
    kill(target, SIGUSR2); 

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