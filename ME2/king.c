#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char secretCode[2];
char childPID[20];
int numberOfTrials, temp;

void createFIFO(char argv[]) {
	char *myfifo = argv;
	mkfifo(myfifo, 0666);
	printf("Pipe creation successful.\n");
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

void obtainChildPID(int fileDescriptor) { read(fileDescriptor, childPID, 255); }

void displayJoinPrompt(int fileDescriptor, char myfifo[]) {
	char entryPrompt[225];
	int breakFlag = 0;
	while(!breakFlag) {
		read(fileDescriptor, entryPrompt, 255);
		strncmp(entryPrompt, "ENTRY", 5) == 0 ?
			printf("%s\n", entryPrompt+5), breakFlag++ : printf("");
	}
}

void sendNumberOfTrials(int fileDescriptor, char myfifo[]) {
	static char numberOfTrialsString[255];
	fileDescriptor = open(myfifo, O_WRONLY);
	snprintf(numberOfTrialsString, 10, "%d", numberOfTrials);
	write(fileDescriptor, numberOfTrialsString, strlen(numberOfTrialsString) + 1);
	close(fileDescriptor);
	// printf("Sent: %s\n", numberOfTrialsString);
}

void killProcess() {
	
}

void listenForContestants(char myfifo[], char secretCode[]) {
	int fileDescriptor;
	char contestantGuess[8];

	sendNumberOfTrials(fileDescriptor, myfifo);

	fileDescriptor = open(myfifo, O_RDONLY);
	resetGuesses();
	obtainChildPID(fileDescriptor);
	displayJoinPrompt(fileDescriptor, myfifo);
	temp--;

	// King = Read-Only
	while(1) {
		read(fileDescriptor, contestantGuess, 8);
		strncmp(secretCode, contestantGuess, 2) == 0 ? 
			printf("%s answers %s correctly!\n", childPID, contestantGuess), initializeSecretCode(), resetGuesses(), temp++ : 
				printf("%s answers %s incorrectly! [%d guesses left.]\n", childPID, contestantGuess, temp), temp--;
		temp > 0 ? 
			printf("") :  
				resetGuesses(), killProcess();
	}
}

int main(int argc, char **argv) {
	char* myfifo = argv[1];
	isPipeNameSpecified(argc, myfifo);
	initializeSecretCode();
	listenForContestants(myfifo, secretCode);
	return 0;
}