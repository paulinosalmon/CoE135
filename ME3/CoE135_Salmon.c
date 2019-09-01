#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define STDIN 0

int main();
void signalHandler(int); 

int playerScore, numberOfQuestions, repeatFlag = 0;
int operandA, operandB, userAnswer, correctAnswer;
struct timeval tv;
fd_set readfds;

void seedRandomizer() { srand(time(NULL)); }

void resetScores() {
	playerScore = 0;
	numberOfQuestions = 0;
}

int randomizeInputs() { return (rand() % 10); }

int getAnswer(int operandA, int operandB) { return operandA + operandB; }

void compareAnswers(int userAnswer, int correctAnswer) {
	userAnswer == correctAnswer ? 
		printf("Correct!\n"), playerScore++ : 
			printf("Incorrect! User input: %d | Correct answer: %d \n", userAnswer, correctAnswer) ;
}

void initiateRandomizer() {

	operandA = randomizeInputs();
	operandB = randomizeInputs();
	correctAnswer = getAnswer(operandA, operandB);
	printf("%d + %d = \n", operandA, operandB);

	numberOfQuestions++;
	
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

			FD_ZERO(&readfds);
			FD_SET(STDIN, &readfds);
			fflush(stdout);

			initiateRandomizer(); 

			select(STDIN+1, &readfds, NULL, NULL, &tv);

			if(FD_ISSET(STDIN, &readfds)) {
				scanf("%d", &userAnswer);
				getchar(); // catch stray newline
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

int main() {

	setbuf(stdout, NULL);
	seedRandomizer();
	signal(SIGINT, signalHandler); 

	while(1) {
		repeatFlag = 0;
		tv.tv_sec = 3;

		FD_ZERO(&readfds);
		FD_SET(STDIN, &readfds);
		fflush(stdout);

		initiateRandomizer(); 

		select(STDIN+1, &readfds, NULL, NULL, &tv);

		if(!repeatFlag) {
			if(FD_ISSET(STDIN, &readfds)) {
				scanf("%d", &userAnswer);
				getchar(); // catch stray newline
				compareAnswers(userAnswer, correctAnswer);
			}
			else 
				printf("Incorrect: Timeout.\n");
		}
	}

	return 0;
}