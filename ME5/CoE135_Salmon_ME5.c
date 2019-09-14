
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const static uint32_t BLOCK_SIZE = 128;
const static uint32_t MAXIMUM_INODE_BLOCKS = 8;

typedef struct {
	uint32_t valid;
	uint32_t size;
	uint32_t direct_ptr[5];
	uint32_t singly_indirect;
} inode_t;

typedef struct {
	uint32_t block_nums[NUM_OF_PTRS];
} indirect_block_t;

// struct FileSystem {

//     ssize_t create();
//     bool    remove(size_t inumber);
//     ssize_t stat(size_t inumber);

//     ssize_t read(size_t inumber, char *data, size_t length, size_t offset);
//     ssize_t write(size_t inumber, char *data, size_t length, size_t offset);
// };

int main() {
	char input[50];

	while(1) {
		printf("$ Enter a command: ");
		fgets(input, 50, stdin);
		printf("\n$ Your input: %s\n", input);
	}



	return 0;
}