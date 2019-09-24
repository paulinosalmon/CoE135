#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>

// To solve for
#define NUM_OF_PTRS 256
#define NUM_OF_INODES_PER_INODE_BLOCK 4
#define MAXIMUM_DIRECT_DATA_BLOCKS 160

// Given
#define BLOCK_SIZE 128
#define MAXIMUM_INODE_BLOCKS 8

char blockCounter = 0;
char inodeCounter = 0;
char nonCorruptedBlocks[30][10];

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

int checkMaximumMemory(int numberOfBlocksNeeded) {

	struct dirent *dStruct;  
	DIR *directory = opendir("files"); 

	if (directory == NULL) { 
		printf("Could not open current directory.\n"); 
		return 0;
	}

	int fileCount = 0;

	while ((dStruct = readdir(directory)) != NULL) {
		if(dStruct->d_name[0] == 'I')
			fileCount++;
	}

	closedir(directory);

	if(numberOfBlocksNeeded > 5 || fileCount == 32+1) {
		printf("Not enough memory left!\n\n");
		return 1;
	}
	else 
		return 0;

}

int checkCorruptedInode() {

	struct dirent *dStruct;  
	DIR *directory = opendir("files"); 

	if (directory == NULL) { 
		printf("Could not open current directory.\n"); 
		exit(1);
	}

	int counter = 0;
	char inodeFileName[20];

	while ((dStruct = readdir(directory)) != NULL) {
		if(dStruct->d_name[0] == 'I') {
			char *str = "files/I";
			char c = dStruct->d_name[1];

			size_t len = strlen(inodeFileName);
			char *str2 = malloc(len + 1 + 1);
			strcpy(str2, str);
			str2[len] = c;
			str2[len + 1] = '\0';

			char string[50] = "files/I";
			strcat(string, str2);
			strcpy(nonCorruptedBlocks[counter], string);
			counter++;
		}
	}

	closedir(directory);
	return counter;
}

void deleteAffectedBlocks(int counter) {

	struct dirent *dStruct;  
	DIR *directory = opendir("files"); 

	if (directory == NULL) { 
		printf("Could not open current directory.\n"); 
		return;
	}
	
	FILE* inodeFile;
	char blocksForThisInode[30][100];
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int numberOfBlocks = 0;

	for(int i = 0; i < counter; i++) {
		inodeFile = fopen(nonCorruptedBlocks[i], "rb");

		if(inodeFile == NULL) 
			return;
		
		
		else {
			while ((read = getline(&line, &len, inodeFile)) != -1) {
				strcpy(blocksForThisInode[numberOfBlocks], choppy(line));
				numberOfBlocks++;
		    }
			fclose(inodeFile);
		}

	    // if(line)
	    //     free(line);

	}

	int safeFlag = 0;

	while ((dStruct = readdir(directory)) != NULL) {
		if(dStruct->d_name[0] == 'B') {

			for(int i = 0; i < numberOfBlocks; i++) {
				int len = strlen(blocksForThisInode[i]);
				const char *blockNumber = &blocksForThisInode[i][len-2];
				const char *blockNumberExtend = &blocksForThisInode[i][len-3];
				if(!(strcmp(blockNumber, dStruct->d_name)) || !(strcmp(blockNumberExtend, dStruct->d_name))) 
					safeFlag = 1;
				
			}

			char prefix[10] = "files/";
			if(!safeFlag) 
				remove(strcat(prefix, dStruct->d_name));
			safeFlag = 0;
		}

	}

	closedir(directory);

}

inode_t* newInode() {
	inode_t *inode = malloc(sizeof(inode_t));
	inode->valid = 1;
	return inode;
}

void w() {
	// deleteAffectedBlocks(checkCorruptedInode());
	blockCounter = 0;
	int fileSize, numberOfBlocksNeeded, pointerCounter = 0;
	FILE *fileInput, *fileOutput, *inodeOutput;

	char inodeFileName[100];
	strcpy(inodeFileName, renameInodeFile());
	while(access(inodeFileName, F_OK) != -1) {
		inodeCounter++;
		strcpy(inodeFileName, renameInodeFile());
	}

	inodeOutput = fopen(inodeFileName, "wb");

	char blocksForThisInode[30][100], fileReader[BLOCK_SIZE], inputFileName[100], outputFileName[100];
	strcpy(outputFileName, renameOutputFile());
	strcpy(blocksForThisInode[pointerCounter], outputFileName);
	fprintf(inodeOutput, "%s\n", blocksForThisInode[pointerCounter]);
	pointerCounter++;

	printf("$ Enter file name: ");
	scanf("%s", inputFileName);
	getchar();

	fileInput = fopen(inputFileName, "rb");
	if(fileInput == NULL) {
		printf("File doesn't exist!\n");
		remove(inodeFileName);
		return;
	}

	fileSize = getFileSize(fileInput);
	rewind(fileInput);
	numberOfBlocksNeeded = getNumberOfBlocksNeededForFile(fileSize);
	if(checkMaximumMemory(numberOfBlocksNeeded)) {
		remove(inodeFileName);
		return;
	}

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
			fprintf(inodeOutput, "%s\n", blocksForThisInode[pointerCounter]);
			pointerCounter++;
			fileOutput = fopen(outputFileName, "wb");
			fseek(fileInput, overallCounter, SEEK_SET);
		}	
	}

	fseek(inodeOutput, 27, SEEK_SET);
    fputc('\0', inodeOutput);

	fclose(fileInput);
	fclose(inodeOutput);

	for(int i = 0; i < pointerCounter; i++) {
		printf("Wrote to %s\n", blocksForThisInode[i]);
	}
	printf("File written with inode number %d\n\n", inodeCounter);
	inodeCounter = 0;

}

void r() {
	// deleteAffectedBlocks(checkCorruptedInode());
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
		printf("Invalid inode number!\n\n");
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
    	if(blockFile == NULL) 
    		printf(" ");
    		
		else {
			while((s = fgetc(blockFile)) != EOF) 
			printf("%c",s);
			fclose(blockFile);
		}
    }
}

void b() {
	// deleteAffectedBlocks(checkCorruptedInode());
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
		fclose(blockFile);
	}
	else 
		printf("File does not exist!\n\n");

}

void d() {
	// deleteAffectedBlocks(checkCorruptedInode());
	int inodeNumber;
	printf("$ Enter inode number: ");
	scanf("%d", &inodeNumber);
	getchar();

	char inodeNumberString[100];
	char inodeFileName[100];

	strcpy(inodeFileName, "files/I");
	sprintf(inodeNumberString, "%d", inodeNumber);
	strcat(inodeFileName, inodeNumberString);

	FILE *inodeFile;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

	inodeFile = fopen(inodeFileName, "rb");

	if(inodeFile == NULL) {
		printf("Invalid inode number!\n\n");
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

    remove(inodeFileName);

    for(int i = 0; i < counter; i++) {
    	char fileName[100];
		strcpy(fileName, choppy(blocksForThisInode[i]));
    	remove(fileName);
    }

	printf("Inode number %d deleted.\n\n", inodeNumber);
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
	// deleteAffectedBlocks(checkCorruptedInode());
	i_inodeCount();
	i_blockCount();
}

// void initializeInodeBlocks() {
// 	FILE* inodeBlockName;

// 	char array[10] = {'0', '1', '2', '3', '4', '5', '6', '7'};

// 	for(int i = 0; i < 8; i++) {
// 		inodeBlockName = fopen(array[i], "w");
// 		write(inodeBlockName, "", 1);
// 		fclose(array[i]);
// 		// printf("%c ", array[i]);
// 	}
// }

int main() {
	char input[50];
	// initializeInodeBlocks();
	deleteAffectedBlocks(checkCorruptedInode());
	while(1) {
		blockCounter = 0;
		inodeCounter = 0;
		printf("\n$ Enter a command: [w/r/b/d/i] ");
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