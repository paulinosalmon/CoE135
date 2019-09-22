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


	while(access(output, F_OK) != -1) {
		blockCounter++;
		strcpy(output, renameOutputFile());
	}
	
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

char *choppy(char *s) {
    char *n = malloc(strlen(s ? s: "\n"));
    if(s)
        strcpy(n, s);
    n[strlen(n) - 1] = '\0';
    return n;
}

inode_t* newInode() {
	inode_t *inode = malloc(sizeof(inode_t));
	inode->valid = 1;
	return inode;
}

void w() {
	blockCounter = 0;
	int fileSize, numberOfBlocksNeeded, pointerCounter = 0;
	FILE *fileInput, *fileOutput, *inodeOutput;

	//////////////////////////////////////////////////////////////////////

	char inodeFileName[100];
	strcpy(inodeFileName, renameInodeFile());
	while(access(inodeFileName, F_OK) != -1) {
		inodeCounter++;
		strcpy(inodeFileName, renameInodeFile());
	}

	inodeOutput = fopen(inodeFileName, "wb");
	printf("Inode #%d is available!\n", inodeCounter);

	//////////////////////////////////////////////////////////////////////

	char blocksForThisInode[30][100], fileReader[BLOCK_SIZE], inputFileName[100], outputFileName[100];
	strcpy(outputFileName, renameOutputFile());
	strcpy(blocksForThisInode[pointerCounter], outputFileName);
	fprintf(inodeOutput, "%s\n", blocksForThisInode[pointerCounter]);
	pointerCounter++;

	//////////////////////////////////////////////////////////////////////


	printf("$ Enter file name: ");
	scanf("%s", inputFileName);
	getchar();

	fileInput = fopen(inputFileName, "rb");
	fileSize = getFileSize(fileInput);
	rewind(fileInput);
	numberOfBlocksNeeded = getNumberOfBlocksNeededForFile(fileSize);

	fileOutput = fopen(outputFileName, "wb");

	int counter = 0, overallCounter = 0;

	//////////////////////////////////////////////////////////////////////

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
			fprintf(inodeOutput, "%s\n", blocksForThisInode[pointerCounter]);
			pointerCounter++;
			fileOutput = fopen(outputFileName, "wb");
			fseek(fileInput, overallCounter, SEEK_SET);
		}	
	}

	fclose(fileInput);
	fclose(inodeOutput);

}


void r() {
	int inodeNumber;
	printf("$ Enter inode number: ");
	scanf("%d", &inodeNumber);
	getchar();

	char inodeNumberString[100];
	char inodeFileName[100];

	strcpy(inodeFileName, "files/I");
	sprintf(inodeNumberString, "%d", inodeNumber);
	strcat(inodeFileName, inodeNumberString);

	FILE *inodeFile, *blockFile;
	char s;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

	inodeFile = fopen(inodeFileName, "rb");

	if(inodeFile == NULL) {
		printf("Invalid inode number!\n");
		return;
	}

	int counter = 0;
	char blocksForThisInode[30][100];

	while ((read = getline(&line, &len, inodeFile)) != -1) {
		strcpy(blocksForThisInode[counter], line);
		counter++;
    }

	fclose(inodeFile);

    if(line)
        free(line);

    for(int i = 0; i < counter; i++) {
    	char fileName[100];
		strcpy(fileName, choppy(blocksForThisInode[i]));
    	blockFile = fopen(fileName, "r");	
		while((s = fgetc(blockFile)) != EOF) 
			printf("%c",s);
		fclose(blockFile);
    }
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

	fclose(blockFile);

}

void d() {

}


void i_inodeCount() {

	struct dirent *dStruct;  
	DIR *directory = opendir("files"); 

	if (directory == NULL) { 
		printf("Could not open current directory.\n"); 
		return;
	}

	int inodeCount = 0;

	while ((dStruct = readdir(directory)) != NULL) {
		if(dStruct->d_name[0] == 'I')
			inodeCount++;
	}

	printf("Valid inode numbers: %d\n", inodeCount);
	closedir(directory);
}

void i_blockCount() {

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
	printf("Used data blocks: %d\n\n", fileCount);
	closedir(directory);
}

void i() {
	i_inodeCount();
	i_blockCount();
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