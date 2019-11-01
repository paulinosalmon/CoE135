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
char seller_ID[50], price[100];
char priceComp1[25], priceComp2[25];

char *choppy(char *s) {
    char *n = malloc(strlen(s ? s: "\n"));
    if(s)
        strcpy(n, s);
    n[strlen(n) - 1] = '\0';
    return n;
}


int plugInID(int pid, char* seller_ID_string) {
    long int seller_ID_int;
    char *pointer;

    seller_ID_int = strtol(seller_ID_string, &pointer, 10);

    if(shmptr->ID1 == 0) {
        shmptr->pid1 = pid; 
        shmptr->ID1 = seller_ID_int;
        return 1;
    }
    
    else if(shmptr->ID4 == 0) {
        shmptr->pid4 = pid; 
        shmptr->ID4 = seller_ID_int;
        return 4;
    }
    
    else if(shmptr->ID6 == 0) {
        shmptr->pid6 = pid; 
        shmptr->ID6 = seller_ID_int;
        return 6;
    }

    else 
        return 0;
    
}

int selectTarget(char* seller_ID_string) {
    long int seller_ID_int;
    int repeatFlag = 0;
    char *pointer;

    seller_ID_int = strtol(seller_ID_string, &pointer, 10);

    if(seller_ID_int == shmptr->ID2) 
        return shmptr->pid2;
    else if (seller_ID_int == shmptr->ID5) 
        return shmptr->pid5;
    else if (seller_ID_int == shmptr->ID7) 
        return shmptr->pid7;
    else 
        return 0;

}


void revertID() {
    if(revert == 1)
        shmptr->ID1 = 0;
    else if(revert == 4)
        shmptr->ID4 = 0;
    else if(revert == 6)
        shmptr->ID6 = 0;
    return;
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
        kill(target, SIGTERM);

        strcpy(shmptr->buff, "Item ");
        strcat(shmptr->buff, seller_ID);
        strcat(shmptr->buff, " Sold.");
        shmptr->commission = commission;

        kill(shmptr->pid3, SIGUSR2);
        revertID();
        shmptr->sellerFlag = 0;
        exit(0);
    }
}

void handler(int signum) {

    if (signum == SIGUSR1) {
        printf("Buyer connected.\n");
        target = selectTarget(seller_ID);
        while(target == 0) {
            target = selectTarget(seller_ID);
        }
    } 

    else if(signum == SIGINT) {
        revertID();
        exit(0);
    }

    // Counter offering communication
    else if (signum == SIGUSR2) {
        printf("\nOffer of "); 
        puts(shmptr->buff); 
        strcpy(priceComp2, choppy(shmptr->buff));

        printf("Counteroffer: "); 
        fgets(shmptr->buff, 44, stdin); 
        strcpy(price, choppy(shmptr->buff));
        strcpy(priceComp1, price);

        kill(target, SIGUSR2); 

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
			(printf("Error, no argument for Seller ID. Exiting...\n"), revertID(), exit(0));
}

char* connectionConfirmed(char argv[]) {
	static char toSend[50];
	strcpy(toSend, "Item ");
	strcat(toSend, argv);
	strcat(toSend, ": Seller connected.");
	return toSend;
}

int main(int argc, char **argv) {

	pid_t pid = getpid(); 
    checkArgs(argc, seller_ID);
    strcpy(seller_ID , argv[1]);
    char* price[100];

    key_t key = ftok("shmfile", 69); 
    shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
    shmptr->sellerFlag = 1;    

    // Initialization Message
    signal(SIGUSR1, handler);   
    signal(SIGUSR2, handler); 
    signal(SIGINT, handler);   
    strcpy(shmptr->buff, connectionConfirmed(seller_ID));
    kill(shmptr->pid3, SIGUSR1); 

    while(shmptr->buyerFlag == 0) {}
    revert = plugInID(pid, seller_ID);

    while (1) {} 

    return 0;

}