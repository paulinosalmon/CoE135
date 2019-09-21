#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

// To solve for
#define NUM_OF_PTRS 8
#define NUM_OF_INODES_PER_INODE_BLOCK 8
#define MAXIMUM_DIRECT_DATA_BLOCKS 29

// Given
#define BLOCK_SIZE 128
#define MAXIMUM_INODE_BLOCKS 8

char blockCounter = 0;
char bitmapForBlocks[30] = "11111111111111111111111111111";

typedef struct {
	uint32_t valid;
	uint32_t size;
	uint32_t direct_ptr[5];
	uint32_t singly_indirect;
} inode_t;

typedef struct {
	uint32_t block_nums[NUM_OF_PTRS];
} indirect_block_t;

int getFileSize(FILE* fileInput) {
	fseek(fileInput, 0L, SEEK_END);
	return ftell(fileInput);
}

int getNumberOfBlocksNeededForFile(int fileSize) {
	return ((fileSize/BLOCK_SIZE) + 1);
}

char* renameOutputFile() {
	static char output[100];
	char blockCounterString[100];

	strcpy(output, "files/B");
	sprintf(blockCounterString, "%d", blockCounter+1);
	strcat(output, blockCounterString);
	
	return output;
}

inode_t* newInode() {
	inode_t *inode = malloc(sizeof(inode_t));
	inode->valid = 1;
	return inode;
}

void w() {
	blockCounter = 0;
	FILE *fileInput;
	FILE *fileOutput;
	int fileSize;
	int numberOfBlocksNeeded;

	char fileReader[BLOCK_SIZE];
	char inputFileName[100];
	char outputFileName[100];
	strcpy(outputFileName, renameOutputFile());

	while(access(outputFileName, F_OK) != -1) {
		blockCounter++;
		strcpy(outputFileName, renameOutputFile());
	}

	printf("$ Enter file name: ");
	scanf("%s", inputFileName);
	getchar();

	fileInput = fopen(inputFileName, "rb");
	fileSize = getFileSize(fileInput);
	rewind(fileInput);
	numberOfBlocksNeeded = getNumberOfBlocksNeededForFile(fileSize);

	fileOutput = fopen(outputFileName, "wb");

	int counter = 0, overallCounter = 0;

	while(1) {
		fread(&fileReader[counter], 1, 1, fileInput);
		printf("Counter at %d\n", counter);
		printf("FILE READER: %s\n", fileReader);
		counter++;
		overallCounter++;

		if(feof(fileInput)) {
			fwrite(fileReader, counter - 1, 1, fileOutput);
			fclose(fileOutput);
			blockCounter = 0;
			break;
		}
		else if((counter % (BLOCK_SIZE) == 0) && (counter != 0)) {
			fread(&fileReader[counter++], 1, 1, fileInput);
			fwrite(fileReader, counter - 1, 1, fileOutput);
			fclose(fileOutput);

			// Reset values
			memset(outputFileName, 0, 100);
			memset(fileReader, 0, BLOCK_SIZE);
			blockCounter++;
			counter = 0;

			strcpy(outputFileName, renameOutputFile());
			fileOutput = fopen(outputFileName, "wb");
			fseek(fileInput, overallCounter, SEEK_SET);
		}	
	}

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

	inode_t *I0 = newInode();
	inode_t *I1 = newInode();
	inode_t *I2 = newInode();
	inode_t *I3 = newInode();
	inode_t *I4 = newInode();
	inode_t *I5 = newInode();
	inode_t *I6 = newInode();
	inode_t *I7 = newInode();

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