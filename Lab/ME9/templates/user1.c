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
    char buff[100]; 
    int status, pid1, pid2; 
}; 
  
struct memory* shmptr; 
  
// handler function to print message received from user2 
  
void handler(int signum) 
{ 
    // if signum is SIGUSR1, then user 1 is receiving a message from user2 
  
    if (signum == SIGUSR1) { 
        printf("Received User2: "); 
        puts(shmptr->buff); 
    } 
} 
  
int main() 
{ 
    // process id of user1 
  
    int pid = getpid(); 
  
    int shmid; 
  
    // key value of shared memory 
    int key = 12345; 
  
    // shared memory create 
    shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666); 
  
    // attaching the shared memory 
  
    shmptr = (struct memory*)shmat(shmid, NULL, 0); 
  
    // store the process id of user1 in shared memory 
    shmptr->pid1 = pid; 
    shmptr->status = NotReady; 
  
    // calling the signal function using signal type SIGUSER1 
    signal(SIGUSR1, handler); 
  
    while (1) { 
        while (shmptr->status != Ready) 
            continue; 
        sleep(1); 
  
        // taking input from user1 
  
        printf("User1: "); 
        fgets(shmptr->buff, 100, stdin); 
  
        shmptr->status = FILLED; 
  
        // sending the message to user2 using kill function 
  
        kill(shmptr->pid2, SIGUSR2); 
    } 
  
    shmdt((void*)shmptr); 
    shmctl(shmid, IPC_RMID, NULL); 
    return 0; 
} 