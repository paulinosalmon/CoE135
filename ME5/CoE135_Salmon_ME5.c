
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// To solve for
#define NUM_OF_PTRS 8
#define NUM_OF_INODES_PER_INODE_BLOCK 8
#define MAXIMUM_DATA_BLOCKS 16

// Given
#define BLOCK_SIZE 128
#define MAXIMUM_INODE_BLOCKS 8

typedef struct {
	uint32_t valid;
	uint32_t size;
	uint32_t direct_ptr[5];
	uint32_t singly_indirect;
} inode_t;

typedef struct {
	uint32_t block_nums[NUM_OF_PTRS];
} indirect_block_t;

void w() {
	char ch[9999];
	FILE *fileInput;
	FILE *fileOutput;
	char inputFileName[100];
	char outputFileName[100] = "files/B1";

	printf("$ Enter file name: ");
	scanf("%s", inputFileName);
	getchar();

	fileInput = fopen(inputFileName, "rb");
	fileOutput = fopen(outputFileName, "wb");

	while(!feof(fileInput)) {
		fread(ch, sizeof(ch), 1, fileInput);
		fwrite(ch, sizeof(char), (strlen(ch)) + 1, fileOutput);
	}

	fclose(fileOutput);
	fclose(fileInput);

}

void r() {

}

void b() {

}

void d() {

}

void i() {

}

int main() {
	char input[50];

	while(1) {
		printf("$ Enter a command: [w/r/b/d/i] ");
		fgets(input, 50, stdin);
		if(!(strncmp(input, "w", 1)))
			w();
		else if(!(strncmp(input, "r", 1)))
			r();
		else if(!(strncmp(input, "b", 1)))
			b();
		else if(!(strncmp(input, "d", 1)))
			d();
		else if(!(strncmp(input, "i", 1)))
			i();
		else 
			printf("Input invalid!\n");
	}

	return 0;
}