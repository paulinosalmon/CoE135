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

char secretCode[2];
char childPID[255];
char parentPID[255];
int numberOfTrials, temp, repeatFlag = 0;
int fileDescriptor;

void createFIFO(char argv[]) {
	char *myfifo = argv;
	mkfifo(myfifo, 0666);
}

void isPipeNameSpecified(int argc, char argv[]) { 
	argc == 2 ? 
		createFIFO(argv) : 
			(printf("Error, no argument for pipe name.\n"), exit(0));
}

int getNumberOfTrials() {
	int numberOfTrials;
	printf("Enter number of trials per contestant: ");
	scanf("%d", &numberOfTrials);
	return numberOfTrials;
}

char* getSecretCode() {
	static char secretCode[2];
	printf("Enter the secret code: ");
	scanf("%s", &secretCode);
	return secretCode;
}

void newGamePrompt(char secretCode[]) {
	printf("Starting a new game with secret code %s.\n", secretCode);
}

void initializeSecretCode() {
	numberOfTrials = getNumberOfTrials();
	strcpy(secretCode, getSecretCode());
	newGamePrompt(secretCode);
}

void resetGuesses() { temp = numberOfTrials; }

void obtainParentPID() { read(fileDescriptor, parentPID, 255); }

void obtainChildPID() { 
	read(fileDescriptor, childPID, 255); 
	!repeatFlag ? obtainParentPID() : printf("");
}

void displayJoinPrompt(char myfifo[]) {
	char entryPrompt[225];
	read(fileDescriptor, entryPrompt, 255);
		strncmp(entryPrompt, "ENTRY", 5) == 0 ?
			printf("%s\n", entryPrompt+5) : printf("");
	
}

void sendNumberOfTrials(char myfifo[]) {
	char numberOfTrialsString[255];
	char secretCodeString[255];
	fileDescriptor = open(myfifo, O_WRONLY);
	snprintf(numberOfTrialsString, 10, "%d", numberOfTrials);
	strcpy(secretCodeString, secretCode);
	strcat(secretCodeString, numberOfTrialsString);
	write(fileDescriptor, secretCodeString, strlen(secretCodeString) + 1);
	close(fileDescriptor);
}

int killProcess(char myfifo[]) {
	pid_t childPIDInt;
	char *pointer;

	close(fileDescriptor);
	childPIDInt = strtol(childPID, &pointer, 10);
	kill(childPIDInt, SIGTERM);
	printf("%d has exceeded number of trials, process will terminated.\n", childPIDInt);
	return 1;
}

void killWinningProcess(char myfifo[]) {
	pid_t childPIDInt;
	char *pointer;

	close(fileDescriptor);
	childPIDInt = strtol(childPID, &pointer, 10);
	kill(childPIDInt, SIGTERM);
}

int checkNumberOfTriesLeft(char myfifo[]) {
	int breakFlag = 0; 
	if(!temp) { resetGuesses(); breakFlag = killProcess(myfifo); } 
	return breakFlag;
}

void recordWinningProcess() {
	FILE *filePointer;
	switch(repeatFlag) {
		case 0: filePointer = fopen("winners.txt", "w");
		default: filePointer = fopen("winners.txt", "a");
	}
	fprintf(filePointer, "%s\n", childPID);
	fclose(filePointer);
}

void resetValues() { memset(childPID, 0, sizeof(childPID)); }

void listenForContestants(char myfifo[], char secretCode[]) {
	char contestantGuess[8];
	int breakFlag = 0;

	sendNumberOfTrials(myfifo);

	fileDescriptor = open(myfifo, O_RDONLY);
	obtainChildPID();
	repeatFlag ? printf("%s has joined the game.\n", childPID) : displayJoinPrompt(myfifo);
	resetGuesses();

	// King = Read-Only
	while(!breakFlag) {
		read(fileDescriptor, contestantGuess, 8);
		if(strncmp(secretCode, contestantGuess, 2) == 0) { 
			printf("%s answers %s correctly!\n", childPID, contestantGuess);
		    recordWinningProcess();
		    initializeSecretCode(); 
		    resetGuesses(); 
		    killWinningProcess(myfifo);
		    breakFlag = 1;
		    temp++; 
		    break;
		} 
		else
			printf("%s answers %s incorrectly! [%d guesses left.]\n", childPID, contestantGuess, temp-1), temp--;
		
		breakFlag = checkNumberOfTriesLeft(myfifo);
	}

	repeatFlag++;
	close(fileDescriptor);
}

int main(int argc, char **argv) {
	char* myfifo = argv[1];
	isPipeNameSpecified(argc, myfifo);
	initializeSecretCode();
	while(1) { listenForContestants(myfifo, secretCode); resetValues(); }
	return 0;
}
