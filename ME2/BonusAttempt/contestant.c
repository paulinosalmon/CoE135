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

#define MAXIMUM_INDEX 65536

char secretCode[MAXIMUM_INDEX];
pid_t pid;
int parentPID, childPID, returnValue, lengthOfSecretCode, numberOfTrials = 1;
int breakFlag = 0;
int repeatFlag = 0;

void initiateFork() {
	if(numberOfTrials > 0 || repeatFlag) { pid = fork(); }
	switch(pid) {
		case -1: perror("fork");
		case 0 : parentPID = getppid();
				 childPID = getpid();
				 repeatFlag ? printf(""): printf("Process id: %d\n",  parentPID) ;
				 returnValue = 44;
		default: wait(&returnValue);
	}
}

char* sendChildPID() {
	static char childPIDString[255];
	snprintf(childPIDString, 10, "%d", childPID);
	return childPIDString;
}

char* sendParentPID() {
	static char parentPIDString[255];
	snprintf(parentPIDString, 10, "%d", parentPID);
	return parentPIDString;
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
	!repeatFlag ? write(fileDescriptor, sendParentPID(), strlen(sendParentPID()) + 1) : printf("") ;
	write(fileDescriptor, sendJoinPrompt(sendChildPID()), strlen(sendJoinPrompt(sendChildPID())) + 1);
}

int getNumberOfTrials(int fileDescriptor, char myfifo[]) {
	long int numberOfTrials;
	char codeAndTrialsMessage[MAXIMUM_INDEX];
	char numberOfTrialsString[MAXIMUM_INDEX];
	char lengthOfSecretCodeString[MAXIMUM_INDEX];
	char *pointer1;
	char *pointer2;

	fileDescriptor = open(myfifo, O_RDONLY);
	read(fileDescriptor, lengthOfSecretCodeString, MAXIMUM_INDEX);
	printf("Received length of code STRING: %s\n", lengthOfSecretCodeString);
	lengthOfSecretCode = strtol(lengthOfSecretCodeString, &pointer2, 10);
	printf("Received length of code: <%d>\n", lengthOfSecretCode);


	read(fileDescriptor, codeAndTrialsMessage, 255);
	strncpy(secretCode, codeAndTrialsMessage, lengthOfSecretCode);
	printf("Received secret code: <%s>\n", secretCode);
	sprintf(numberOfTrialsString, "%s", codeAndTrialsMessage+lengthOfSecretCode);
	numberOfTrials = strtol(codeAndTrialsMessage+lengthOfSecretCode, &pointer1, 10);
	printf("Received number of trials: %d\n", numberOfTrials);
	close(fileDescriptor);
	return numberOfTrials;
}

void turnOnBreakFlag(int numberOfTrials) { 
	breakFlag = 1; 
	numberOfTrials > 0 ? printf("%d won.\n", childPID): printf("%d lost.\n", childPID);
}

void signalHandler(int sig) { signal(sig, SIG_IGN); }

void recordWinningProcess() {
	FILE *filePointer;
	char childPIDString[255];

	switch(repeatFlag) {
		case 0: filePointer = fopen("players.txt", "w");
		default: filePointer = fopen("players.txt", "a");
	}
	snprintf(childPIDString, 10, "%d", childPID);
	fprintf(filePointer, "%s - W\n", childPIDString);
	fclose(filePointer);
}

void recordLosingProcess() {
	FILE *filePointer;
	char childPIDString[255];

	switch(repeatFlag) {
		case 0: filePointer = fopen("players.txt", "w");
		default: filePointer = fopen("players.txt", "a");
	}
	snprintf(childPIDString, 10, "%d", childPID);
	fprintf(filePointer, "%s - L\n", childPIDString);
	fclose(filePointer);
}

void spawnContestants(char myfifo[]) {
	int fileDescriptor, temp;
	char contestantGuess[MAXIMUM_INDEX];

	initiateFork();
	numberOfTrials = getNumberOfTrials(fileDescriptor, myfifo);
	temp = numberOfTrials + 1;

	fileDescriptor = open(myfifo, O_WRONLY);
	sendEntryMessage(fileDescriptor);

	// Contestant = Write-Only
	while(!breakFlag) {
		signal(SIGTERM, signalHandler);
		printf("%d Please guess the code: ", childPID);
		scanf("%[^\n]", &contestantGuess);
		write(fileDescriptor, contestantGuess, strlen(contestantGuess) + 1);
		printf("Sent: |%s|\n", contestantGuess);
		if(strncmp(secretCode, contestantGuess, lengthOfSecretCode) == 0) {
			printf("GUESSED CORRECTLY!\n");
			scanf("%s GUESSED CORRECTLY!\n", &contestantGuess);
			recordWinningProcess();
			turnOnBreakFlag(numberOfTrials);
			break;
		}

		printf("GUESSED WRONG!\n");
		scanf("%s GUESSED WRONG!\n", &contestantGuess);
		numberOfTrials--;
		switch(numberOfTrials) { case 0: breakFlag ? printf("") : recordLosingProcess(), turnOnBreakFlag(numberOfTrials); }
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
	while(1) { spawnContestants(argv[1]); repeatFlag = 1; }
	return 0;
}
