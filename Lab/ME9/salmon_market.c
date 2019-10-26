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
	// Initial Message/Logs
    if (signum == SIGUSR1) 
		puts(shmptr->buff); 
	// Logs
	else if (signum == SIGUSR2) {

	}
} 

int main() {
	int pid = getpid(); 

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, sizeof(struct memory), 0666|IPC_CREAT); 
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 

    printf("Market pool: 0\n");
    printf("Waiting for connections...\n");

    shmptr->pid3 = pid; 
    shmptr->status = NotReady;    
    signal(SIGUSR1, handler);     
    signal(SIGUSR2, handler);  

    // Listen to connections
    while(1) {
        while (shmptr->status != Ready) 
            continue; 
        // sleep(1); 
  
        shmptr->status = FILLED; 
	}

    shmdt((void*)shmptr); 
    shmctl(shmid, IPC_RMID, NULL); 
    return 0; 

}