#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

pid_t pid;
int parentPID, childPID, returnValue;
int breakFlag = 0;

void initiateFork() {
	pid = fork();
	switch(pid) {
		case -1: perror("fork");
		case 0 : parentPID = getppid();
				 childPID = getpid();
				 printf("Created new child process.\n");
				 returnValue = 44;
		default: wait(&returnValue);
	}
}

char* sendChildPID() {
	static char childPIDString[255];
	snprintf(childPIDString, 10, "%d", childPID);
	return childPIDString;
}

char* sendJoinPrompt(char childPIDString[]) {	
	static char entryFlag[255];
	strcpy(entryFlag, "ENTRY");
	char *entryPrompt = " has joined the game.\n";
	strcat(childPIDString, entryPrompt);
	strcat(entryFlag, childPIDString);
	return entryFlag;
}

void sendEntryMessage(int fileDescriptor) {
	write(fileDescriptor, sendChildPID(), strlen(sendChildPID()) + 1);
	write(fileDescriptor, sendJoinPrompt(sendChildPID()), strlen(sendJoinPrompt(sendChildPID())) + 1);
}

int getNumberOfTrials(int fileDescriptor, char myfifo[]) {
	long int numberOfTrials;
	char numberOfTrialsString[255];
	char *pointer;

	fileDescriptor = open(myfifo, O_RDONLY);
	read(fileDescriptor, numberOfTrialsString, 255);
	numberOfTrials = strtol(numberOfTrialsString, &pointer, 10);
	close(fileDescriptor);
	return numberOfTrials;
}

void turnOnBreakFlag() { breakFlag = 1; printf("%d lost.\n", childPID); }

void signalHandler(int sig) { signal(sig, SIG_IGN); turnOnBreakFlag(); }

void spawnContestants(char myfifo[]) {
	int fileDescriptor, numberOfTrials, temp;
	char contestantGuess[8];

	initiateFork();
	numberOfTrials = getNumberOfTrials(fileDescriptor, myfifo);
	temp = numberOfTrials + 1;

	fileDescriptor = open(myfifo, O_WRONLY);
	sendEntryMessage(fileDescriptor);

	// Contestant = Write-Only
	while(!breakFlag) {
		printf("%d Please guess the code: ", childPID);
		scanf("%s", &contestantGuess);
		signal(SIGINT, signalHandler);
		write(fileDescriptor, contestantGuess, strlen(contestantGuess) + 1);
		numberOfTrials--;
		switch(numberOfTrials) { case 0: turnOnBreakFlag(); }
	}

	close(fileDescriptor);
	breakFlag = 0;
}

void isPipeNameSpecified(int argc, char argv[]) { 
	argc == 2 ? 
		 printf("") : 
			(printf("Error, no argument for pipe name.\n"), exit(0));
}

int main(int argc, char **argv) {
	char* myfifo = argv[1];
	isPipeNameSpecified(argc, myfifo);
	while(1) { spawnContestants(argv[1]); }
	return 0;
}
