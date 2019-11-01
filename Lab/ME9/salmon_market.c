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
  
struct memory* shmptr; 
int marketTotal = 0;
int shmid;

void handler(int signum) {
	// Initial Message/Logs 
    if (signum == SIGUSR1) 
		puts(shmptr->buff); 

	// Logs
	else if (signum == SIGUSR2) {
		printf("%s %d commission.\n", shmptr->buff, shmptr->commission);
		marketTotal = marketTotal + shmptr->commission;
		printf("Market pool: %d\n", marketTotal);

	}

    else if(signum == SIGINT) {
        shmdt((void*)shmptr); 
        shmctl(shmid, IPC_RMID, NULL); 
        exit(0);
    }

} 

void initialize() {
    shmptr->pid1 = 0;
    shmptr->pid2 = 0;
    shmptr->pid4 = 0;
    shmptr->pid5 = 0;
    shmptr->pid6 = 0;
    shmptr->pid7 = 0;

    shmptr->ID1 = 0;
    shmptr->ID2 = 0;
    shmptr->ID4 = 0;
    shmptr->ID5 = 0;
    shmptr->ID6 = 0;
    shmptr->ID7 = 0;
}

int main() {
	pid_t pid = getpid(); 

    key_t key = ftok("shmfile", 69); 
    shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
    initialize();

    printf("Market pool: 0\n");
    printf("Waiting for connections...\n");

    shmptr->pid3 = pid;  
    signal(SIGUSR1, handler);     
    signal(SIGUSR2, handler);
    signal(SIGINT, handler);     

    // Listen to connections
    while(1) {}

    return 0; 

}