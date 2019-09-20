#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define STDIN 0

int playerScore, numberOfQuestions, repeatFlag = 0;
int operandA, operandB, userAnswer, correctAnswer, operation, undefinedFlag = 0, indeterminateFlag = 0;
char userAnswerString[255];
char operationSign[5];
struct timeval tv;
fd_set readfds;
static struct termios old, current;

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

int getAnswer(int operandA, int operandB, int operation) { 
	switch(operation) {
		case 0: return operandA + operandB; 
		case 1: return operandA - operandB; 
		case 2: return operandA * operandB; 
		default: return operandA / operandB; 
	}
}

void checkSpecialExceptions() {
	if(operandA == 0 && operandB == 0 && operation == 3) {
		correctAnswer = 57;	// int value of i
		indeterminateFlag = 1;
	}
	else if(operandB == 0 && operation == 3) {
		correctAnswer = 69;	// int value of u
		undefinedFlag = 1;
	}
	else 
		correctAnswer = getAnswer(operandA, operandB, operation);
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
	if(userAnswer == correctAnswer) { 
		printf("Correct!\n");
		playerScore++;
	}
	else {
		if(indeterminateFlag) {
			printf("Incorrect! User input: %d | Correct answer: i \n", userAnswer);
			indeterminateFlag = 0;
		}
		else if(undefinedFlag) {
			printf("Incorrect! User input: %d | Correct answer: u \n", userAnswer);
			undefinedFlag = 0;
		}
		else
			printf("Incorrect! User input: %d | Correct answer: %d \n", userAnswer, correctAnswer) ;
	}
}

void initiateRandomizer() {
	operandA = randomizeInputs();
	operandB = randomizeInputs();
	operation = randomizeOperation();

	printf("%d %s %d = \n", operandA, getOperationSign(), operandB);
	checkSpecialExceptions();
	
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

void initTermios(int echo) {
	tcgetattr(0, &old); 
	current = old; 
	current.c_lflag &= ~ICANON; 
	if (echo) 
		current.c_lflag |= ECHO; 
	else 
		current.c_lflag &= ~ECHO; 

	tcsetattr(0, TCSANOW, &current); 
}

void resetTermios(void) { tcsetattr(0, TCSANOW, &old); }

char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

char getch(void) { return getch_(0); }

char getche(void) { return getch_(1); }

int checkQuitPrompt() {
	char c;
	c = getche();

	while(1) {
		if(c == 'y' || c == 'Y')
			exit(0);
		else if(c == '\n') {
			repeatFlag = 1;
			return 1;
		}
		else {
			printf("\nInvalid input. Try again.\n");
			c = getche();
		}
	}
}

void checkForBlankInput() {
	if(userAnswerString[0] == '\n') {
		if(indeterminateFlag) {
			printf("Incorrect!  NO INPUT DETECTED | Correct answer: i \n");
			indeterminateFlag = 0;
		}
		else if(undefinedFlag) {
			printf("Incorrect!  NO INPUT DETECTED | Correct answer: u \n");
			undefinedFlag = 0;
		}
		else
			printf("Incorrect! NO INPUT DETECTED | Correct answer: %d \n", correctAnswer);
	}
	else {
		userAnswer = myAtoi(userAnswerString);
		compareAnswers(userAnswer, correctAnswer);
	}
}

void signalHandler(int sig) { 

	fflush(stdout);
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);

	printf("\nYou got %d out of %d items correctly.\n", playerScore, numberOfQuestions-1);
	resetScores();
	printf("\nPress y or Y if you want to finish the game. Press enter to start a new one.\n");

	if(checkQuitPrompt()) {
		tv.tv_sec = 3;
		signal(SIGINT, signalHandler); 

		initiateRandomizer(); 
		select(STDIN+1, &readfds, NULL, NULL, &tv);

		if(FD_ISSET(STDIN, &readfds)) {
			fgets(userAnswerString, 255, stdin);
			checkForBlankInput();
		}
		else {
			printf("Incorrect: Timeout.\n");
			repeatFlag = 1;
			return;
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
			checkForBlankInput();
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
