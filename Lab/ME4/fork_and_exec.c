#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void initiateFork() {

	pid_t pid;
	int returnValue;

	switch(pid = fork()) {
		case -1: 
				perror("Fork error.\n");
				exit(1);

		case 0: 
				printf("CHILD\n");
				printf("\tPID IS: %d\n", getpid());
				printf("\tPARENT PID IS: %d\n", getppid());
				printf("\tSetting status exit to 44...\n");
				returnValue = 44;
				printf("\tCHILD: Time to rest.\n");
				exit(returnValue);

		default: 
				printf("PARENT\n");
				printf("\tPID IS: %d\n", getpid());
				printf("\tPARENT PID IS: %d\n", getppid());
				printf("\tWaiting for child to terminate...\n");
				wait(&returnValue);
				printf("\tChild exited with status: %d\n", WEXITSTATUS(returnValue));
				printf("\tPARENT: Time to rest.\n");
	}
}

void initiateExec() {
	printf("\nCalling 'cowsay' with execvp()...\n");
	char *args[]={"cowsay", "\"KIM CHAEWON BEST GIRL\"" ,NULL}; 
	execvp(args[0], args); 
}

int main() {

	initiateFork();
	initiateExec();

	return 0;
}