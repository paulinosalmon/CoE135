#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
  
int main() { 

    key_t key = ftok("shmfile", 69); 
    int shmid = shmget(key, 1024, 0666|IPC_CREAT); 
    char *str = (char*) shmat(shmid,(void*) 0, 0); 
  
    printf("Data read from memory: %s\n", str); 
      
    //detach from shared memory 
    shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid, IPC_RMID, NULL); 
     
    return 0; 
} 