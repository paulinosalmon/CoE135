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
  
struct memory { 
    char buff[44]; 
    int buyerFlag, sellerFlag;
    int ID1, ID2, ID4, ID5, ID6, ID7;
    int status, pid1, pid2, pid3, pid4, pid5, pid6, pid7; 
    int commission;
}; 
  
int target, revert, shmid;
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


int plugInID(int pid, char* buyer_ID_string) {
    long int buyer_ID_int;
    char *pointer;

    buyer_ID_int = strtol(buyer_ID_string, &pointer, 10);

    if(shmptr->ID2 == 0) {
        shmptr->pid2 = pid; 
        shmptr->ID2 = buyer_ID_int;
        return 2;
    }
    
    else if(shmptr->ID5 == 0) {
        shmptr->pid5 = pid; 
        shmptr->ID5 = buyer_ID_int;
        return 5;
    }

    else if(shmptr->ID7 == 0) {
        shmptr->pid7 = pid; 
        shmptr->ID7 = buyer_ID_int;
        return 7;
    }

    else 
        return 0;

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
        return shmptr->pid6;    
    else {
        return 0;
    }

}

void revertID() {
    if(revert == 2)
        shmptr->ID2 = 0;
    else if(revert == 5)
        shmptr->ID5 = 0;
    else if(revert == 7)
        shmptr->ID7 = 0;
    return;
}

void priceCompare(char *first, char *second) {
    if(!strcmp(first, second)) {
        printf("You've now bought Item %s.\n", buyer_ID);
        revertID();
        shmptr->buyerFlag = 0;
        revertID();
        exit(0);
    }
}
  
void handler(int signum) {  

    if (signum == SIGUSR1) {}

    // Counter offering communication
    else if (signum == SIGUSR2) {
        printf("\nCounteroffer of "); 
        puts(shmptr->buff); 
        strcpy(priceComp2, choppy(shmptr->buff));

        priceCompare(priceComp1, priceComp2);

        printf("Offer: "); 
        fgets(shmptr->buff, 44, stdin); 
        strcpy(price, choppy(shmptr->buff));
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
        revertID();
        exit(0);
    }

    else if(signum == SIGINT) {
        revertID();
        exit(0);
    }

} 

void createSeller() {}

void checkArgs(int argc, char argv[]) { 
	argc == 2 ? 
		createSeller() : 
			(printf("Error, no argument for Buyer ID. Exiting...\n"), revertID(), exit(0));
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

	pid_t pid = getpid();
	strcpy(buyer_ID, argv[1]);

    key_t key = ftok("shmfile", 69); 
    shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
    shmptr->buyerFlag = 1;

    printf("Waiting for seller...\n"); 
    signal(SIGUSR1, handler); 
    signal(SIGUSR2, handler); 
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    strcpy(shmptr->buff, connectionConfirmed(buyer_ID));
    kill(shmptr->pid3, SIGUSR1); 

    revert = plugInID(pid, buyer_ID);
    while(revert == 0) 
        revert = plugInID(pid, buyer_ID);
    
    target = selectTarget(buyer_ID);
    while(target == 0) 
        target = selectTarget(buyer_ID);

    while(shmptr->sellerFlag == 0) {}
        
    printf("Seller connected.\n");

    // Initialization Message
    kill(target, SIGUSR1); 
    printf("\nOffer: "); 
    fgets(shmptr->buff, 44, stdin); 
    strcpy(price, choppy(shmptr->buff));
    strcpy(priceComp1, price);
    kill(target, SIGUSR2); 

    sleep(1);
    strcpy(shmptr->buff, "Item ");
    strcat(shmptr->buff, buyer_ID);
    strcat(shmptr->buff, ": Offer ");
    strcat(shmptr->buff, price);
    kill(shmptr->pid3, SIGUSR1); 

    while (1) {} 

    return 0;

}