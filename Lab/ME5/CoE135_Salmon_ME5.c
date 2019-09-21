#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h> 

// To solve for
#define NUM_OF_PTRS 8
#define NUM_OF_INODES_PER_INODE_BLOCK 8
#define MAXIMUM_DIRECT_DATA_BLOCKS 29

// Given
#define BLOCK_SIZE 128
#define MAXIMUM_INODE_BLOCKS 8

char blockCounter = 0;
char inodeCounter = 0;
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

char* renameInodeFile() {
	static char output[100];
	char inodeCounterString[100];

	strcpy(output, "files/I");
	sprintf(inodeCounterString, "%d", inodeCounter);
	strcat(output, inodeCounterString);
	
	return output;
}

inode_t* newInode() {
	inode_t *inode = malloc(sizeof(inode_t));
	inode->valid = 1;
	return inode;
}

void w() {
	blockCounter = 0;
	int fileSize, numberOfBlocksNeeded, pointerCounter = 0;
	FILE *fileInput, *fileOutput;

	char blocksForThisInode[30][100], fileReader[BLOCK_SIZE], inputFileName[100], outputFileName[100];
	strcpy(outputFileName, renameOutputFile());
	strcpy(blocksForThisInode[pointerCounter], outputFileName);
	pointerCounter++;

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
			strcpy(blocksForThisInode[pointerCounter], outputFileName);
			pointerCounter++;
			fileOutput = fopen(outputFileName, "wb");
			fseek(fileInput, overallCounter, SEEK_SET);
		}	
	}

	fclose(fileInput);

	char inodeFileName[100];
	strcpy(inodeFileName, renameInodeFile());
	while(access(inodeFileName, F_OK) != -1) {
		inodeCounter++;
		strcpy(inodeFileName, renameInodeFile());
	}

	// printf("Inode #%d is available!\n", inodeCounter);

	// for(int i = 0; i < MAXIMUM_DIRECT_DATA_BLOCKS; i++) {
	// 	for(int j = 0; j < 255; j++) {
	// 		printf("%c", blocksForThisInode[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// inodeOutput = fopen(inodeFileName, "wb");
	// for (int *ip = &blocksForThisInode[0][0]; ip <= &blocksForThisInode[MAXIMUM_DIRECT_DATA_BLOCKS][255]; ip++)
	// 	printf("%c", *ip);

}


void r() {
}

void b() {
	int blockNumber;
	char blockFileName[100], blockNumberString[100];
	FILE *blockFile;
	printf("$ Enter block number: ");
	scanf("%d", &blockNumber);
	getchar();

	strcpy(blockFileName, "files/B");
	sprintf(blockNumberString, "%d", blockNumber);
	strcat(blockFileName, blockNumberString);

	if(access(blockFileName, F_OK) != -1) {
		char s;
		blockFile = fopen(blockFileName, "r");
		while((s = fgetc(blockFile)) != EOF) 
			printf("%c",s);
		printf("\n\n");
	}
	else 
		printf("File does not exist!\n");

}

void d() {

}

void i() {
	struct dirent *dStruct;  
	DIR *directory = opendir("files"); 

	if (directory == NULL) { 
		printf("Could not open current directory.\n"); 
		return;
	}

	int fileCount = 0;

	while ((dStruct = readdir(directory)) != NULL) {
		if(dStruct->d_name[0] == 'B')
			fileCount++;
	}

	printf("Valid inode numbers: \n");
	printf("Used data blocks: %d\n\n", fileCount);
	closedir(directory);
}

int main() {
	char input[50];

	// inode_t *I0 = newInode();
	// inode_t *I1 = newInode();
	// inode_t *I2 = newInode();
	// inode_t *I3 = newInode();
	// inode_t *I4 = newInode();
	// inode_t *I5 = newInode();
	// inode_t *I6 = newInode();
	// inode_t *I7 = newInode();

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