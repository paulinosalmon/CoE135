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

int playerScore, numberOfQuestions, breakFlag = 0;

void seedRandomizer() { srand(time(NULL)); }

void resetScores() {
	playerScore = 0;
	numberOfQuestions = 0;
}

void checkQuitPrompt(char quitPrompt[]) {
	strncmp(quitPrompt, "Y", 1);
	if(!(strncmp(quitPrompt, "Y", 1)) || !(strncmp(quitPrompt, "y", 1)))
		breakFlag = 1;
	else if(!(strncmp(quitPrompt, "\n", 1)))
		breakFlag = 0;
}

void signalHandler(int sig) { 
	char quitPrompt[10];

	signal(SIGINT, signalHandler); 
	printf("\nYou got %d out of %d items correctly.\n", playerScore, numberOfQuestions);
	resetScores();
	printf("\nPress y or Y if you want to finish the game. Press enter to start a new one. \n"); 
    scanf("%s", &quitPrompt);
    checkQuitPrompt(quitPrompt);
}

int randomizeInputs() { return (rand() % 10); }

int getAnswer(int operandA, int operandB) { return operandA + operandB; }

void compareAnswers(int userAnswer, int correctAnswer) {
	userAnswer == correctAnswer ? 
		printf("Correct!\n"), playerScore++, numberOfQuestions++ : 
			printf("Incorrect! User input: %d | Correct answer: %d \n", userAnswer, correctAnswer), numberOfQuestions++ ;
}

void initiateRandomizer() {
	int operandA, operandB, userAnswer, correctAnswer;
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec = 3;

	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);

	operandA = randomizeInputs();
	operandB = randomizeInputs();
	correctAnswer = getAnswer(operandA, operandB);

	printf("%d + %d = \n", operandA, operandB);
	select(STDIN+1, &readfds, NULL, NULL, &tv);

	if(FD_ISSET(STDIN, &readfds)) {
		scanf("%d", &userAnswer);
		compareAnswers(userAnswer, correctAnswer);
	}
	else {
		printf("Incorrect: Timeout.\n");
		numberOfQuestions++;
	}
}

int main() {

	seedRandomizer();

	while(1) { 
		signal(SIGINT, signalHandler); 
		initiateRandomizer(); 
		if(breakFlag)
			break;
	}

	return 0;
}