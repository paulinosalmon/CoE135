#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define STDIN 0

int playerScore, numberOfQuestions, repeatFlag = 0;
int operandA, operandB, userAnswer, correctAnswer, operation;
char userAnswerString[255];
char operationSign[5];
struct timeval tv;
fd_set readfds;

void seedRandomizer() { srand(time(NULL)); }

int randomizeInputs() { return (rand() % 10); }

int randomizeOperation() { return (rand() % 4); }

void resetScores() {
	playerScore = 0;
	numberOfQuestions = 0;
	operandA = 0;
	operandB = 0;
	userAnswer = 0;
	correctAnswer = 0;
	operation = 0;
}

void divisionByZero() {
	while(operation == 4 && operandB == 0) 
		operandB = randomizeInputs();
}

int getAnswer(int operandA, int operandB, int operation) { 
	switch(operation) {
		case 0: return operandA + operandB; 
		case 1: return operandA - operandB; 
		case 2: return operandA * operandB; 
		default: return operandA / operandB; 
	}
}

char* getOperationSign() {
	switch(operation) {
		case 0: return "+"; 
		case 1: return "-"; 
		case 2: return "*"; 
		default: return "/"; 
	}
}

void compareAnswers(int userAnswer, int correctAnswer) {
	userAnswer == correctAnswer ? 
		printf("Correct!\n"), playerScore++ : 
			printf("Incorrect! User input: %d | Correct answer: %d \n", userAnswer, correctAnswer) ;
}

void initiateRandomizer() {
	operandA = randomizeInputs();
	operandB = randomizeInputs();
	operation = randomizeOperation();

	divisionByZero();
	
	correctAnswer = getAnswer(operandA, operandB, operation);
	printf("%d %s %d = \n", operandA, getOperationSign(), operandB);
	numberOfQuestions++;
}

int myAtoi(char* str) {
	int convertedInt = 0;

	if(str[0] == '-') {
		for(int i = 1; i < strlen(str) - 1; ++i) 
			convertedInt = convertedInt * 10 + str[i] - '0';
		return convertedInt * -1;
	}
	else {
		for(int i = 0; i < strlen(str) - 1; ++i) 
			convertedInt = convertedInt * 10 + str[i] - '0';
		return convertedInt;
	}

}

int checkQuitPrompt(int quitPrompt) {
	if(quitPrompt == 89 || quitPrompt == 121)
		exit(0);
	else if(quitPrompt == 10) {
		repeatFlag = 1;
		return 1;
	}
	else {
		printf("Invalid input. Terminating program...\n");
		exit(0);
	}
	return 0;
}

void signalHandler(int sig) { 
	fflush(stdout);
	int quitPrompt;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);

	printf("\nYou got %d out of %d items correctly.\n", playerScore, numberOfQuestions-1);
	resetScores();
	printf("\nPress y or Y if you want to finish the game. Press enter to start a new one.\n");
	select(STDIN+1, &readfds, NULL, NULL, NULL);

	if(FD_ISSET(STDIN, &readfds)) {
		quitPrompt = getchar();
		if(checkQuitPrompt(quitPrompt)) {
			tv.tv_sec = 3;
			signal(SIGINT, signalHandler); 

			initiateRandomizer(); 
			select(STDIN+1, &readfds, NULL, NULL, &tv);

			if(FD_ISSET(STDIN, &readfds)) {
				fgets(userAnswerString, 255, stdin);
				userAnswer = myAtoi(userAnswerString);
				compareAnswers(userAnswer, correctAnswer);
			}
			else {
				printf("Incorrect: Timeout.\n");
				repeatFlag = 1;
				return;
			}

		}
	}
}

void listenForInput() {

	tv.tv_sec = 3;

	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);
	fflush(stdout);

	select(STDIN+1, &readfds, NULL, NULL, &tv);

	if(!repeatFlag) {
		if(FD_ISSET(STDIN, &readfds)) {
			fgets(userAnswerString, 255, stdin);
			userAnswer = myAtoi(userAnswerString);
			compareAnswers(userAnswer, correctAnswer);
		}
		else 
			printf("Incorrect: Timeout.\n");
	}
}

int main() {

	setbuf(stdout, NULL);
	seedRandomizer();
	signal(SIGINT, signalHandler); 

	while(1) {
		repeatFlag = 0;
		initiateRandomizer(); 
		listenForInput();
	}

	return 0;
}
