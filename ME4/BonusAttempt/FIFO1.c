#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int fileDescriptor;
int skipFlag = 0;
char* myfifo = "bonus"; 
char userPID[80]; 

void writeMessage() { 
    char arr1[80];
    fileDescriptor = open(myfifo, O_WRONLY); 
    printf("\nYou: ");
    fgets(arr1, 80, stdin); 
    if(!(strncmp(arr1, "exit()", 6))) {
        printf("\nTerminating chat...\n");
        exit(0);
    }
    write(fileDescriptor, arr1, strlen(arr1)+1); 
    close(fileDescriptor); 
}

void readMessage() {
    char arr2[80]; 
    fileDescriptor = open(myfifo, O_RDONLY); 
    read(fileDescriptor, arr2, sizeof(arr2)); 
    if(!skipFlag)
        printf("\n%s: %s", userPID, arr2); 
    close(fileDescriptor); 
}

void getPID() {
    fileDescriptor = open(myfifo, O_RDONLY); 
    read(fileDescriptor, userPID, sizeof(userPID)); 
    close(fileDescriptor);
    printf("You are now chatting with User PID#%s!\n", userPID);
    printf("Type exit() when prompted to write a message to quit chat.\n"); 
    printf("Press CTRL + C to kick the other user out.\n");   
    printf("Press CTRL + Z to mute the other user.\n");   
}

void sendPID() {
    int ownPID = getpid();
    char ownPIDString[80];
    fileDescriptor = open(myfifo, O_WRONLY); 
    snprintf(ownPIDString, 10, "%d", ownPID);
    write(fileDescriptor, ownPIDString, strlen(ownPIDString)+1); 
    close(fileDescriptor); 
}

void initiateFIFO() {
    mkfifo(myfifo, 0666); 
    printf("Listening for connections...\n");
    getPID();
    sendPID();
}

void signalHandlerCtrlC(int sig) { 
    char arr1[80];
    pid_t userPIDInt;
    char *pointer;
    userPIDInt = strtol(userPID, &pointer, 10);

    signal(sig, SIG_IGN);
    printf("\nKicked User PID#%s from chat.\n", userPID);
    kill(userPIDInt, SIGTERM);
    printf("Type exit() to exit too.");
    while(1) {
        fgets(arr1, 80, stdin); 
        if(!(strncmp(arr1, "exit()", 6))) 
            exit(0);
        printf("Invalid command.\n");
    }
}

void signalHandlerCtrlZ(int sig) { 
    char arr1[80];
    printf("\nUser PID#%s has been muted.\n", userPID);
    printf("Type '/unmute' anytime to resume operation.\n");
    signal(sig, SIG_IGN);

    while(1) {
        fgets(arr1, 80, stdin); 
        if(!(strncmp(arr1, "/unmute", 7))) {
            printf("\nResuming chat...\n");
            break;
        }
    }
    skipFlag = 1;
}

int main() { 
    signal(SIGINT, signalHandlerCtrlC); 
    signal(SIGTSTP, signalHandlerCtrlZ); 
    initiateFIFO();

    while (1) {
        writeMessage();
        readMessage();  
    } 

    return 0; 
} 