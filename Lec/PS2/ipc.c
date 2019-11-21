#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#define PAGESIZE 4096
#define CTRMAX 2000000

#define increment_val(ptr) ptr += 1

int main() {

	key_t key;
	key = ftok("probset", 65);

	uint32_t smem = msgget(key, 0666 | IPC_CREAT); 

	smem = 10;

	pid_t pid;

	pid = fork();
	assert(pid >= 0);

	if (pid) {
		for (int ctr = 0; ctr < CTRMAX; ++ctr)
			increment_val(smem);
		wait(NULL);
	} 
	else {
		for (int ctr = 0; ctr < CTRMAX; ++ctr)
			increment_val(smem);
	}

	if (pid) {
		printf("Data: %d\n", smem);
		msgctl(smem, IPC_RMID, NULL);
	}

	return 0;
}