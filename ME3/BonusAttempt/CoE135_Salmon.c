#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void seedRandomizer() { srand(time(NULL)); }

void signalHandler(int sig) { 
	signal(SIGINT, signalHandler); 
	printf("\nPress y or Y if you want to finish the game. Press enter to start a new one. \n"); 
    fflush(stdout); 
}

int randomizeInputs() { return (rand() % 10); }

int getAnswer(int operandA, int operandB) { return operandA + operandB; }

void compareAnswers(int userAnswer, int correctAnswer) {
	userAnswer == correctAnswer ? 
		printf("Correct!\n") : 
			printf("Incorrect! User input: %d | Correct answer: %d \n", userAnswer, correctAnswer);
}

void initiateRandomizer() {
	int operandA, operandB, userAnswer, correctAnswer;

	operandA = randomizeInputs();
	operandB = randomizeInputs();
	correctAnswer = getAnswer(operandA, operandB);
	printf("%d + %d = \n", operandA, operandB);
	scanf("%d", &userAnswer);
	compareAnswers(userAnswer, correctAnswer);
}

int main() {

	seedRandomizer();

	while(1) { 
		signal(SIGINT, signalHandler); 
		initiateRandomizer(); 
	}

	return 0;
}