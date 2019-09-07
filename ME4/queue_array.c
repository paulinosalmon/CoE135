#include <stdio.h>
#include <stdlib.h>

int globalLineNumber = 0;

void printQueue(int queue[], size_t queueSize) {
    for(int i = 0; i < queueSize; i++) 
        printf("%d ", queue[i]);
}

void insertInt(int queue[], size_t queueSize) {
    printf("Enter integer to insert: ");
    scanf("%d", &queue[globalLineNumber]);
    printf("Inserted. Queue is now: \n");
    printQueue(queue, queueSize);

    if(globalLineNumber != 10)
        globalLineNumber++;
}

void popInt(int queue[], size_t queueSize) {
    printf("Popping first in queue...\n");
    queue[0] = 0;

    for(int i = 0; i < queueSize - 1; i++) {
        queue[i] = queue[i + 1];
    }
    queue[queueSize - 1] = 0;

    if(globalLineNumber != 0)
        globalLineNumber--;
    
    printf("Popped. Queue is now:\n");
    printQueue(queue, queueSize);
}

int main() {

    int queue[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t queueSize = sizeof(queue)/sizeof(queue[0]);
    int choice;

    while(1) {
        printf("\nWhat do you want to do?\n1) Insert integer.\n2) Pop integer.\n3) Exit\n");
        scanf("%d", &choice);
        switch(choice) {
            case 1: 
                insertInt(queue, queueSize);
                break;
            case 2:
                popInt(queue, queueSize);
                break;
            default:
                exit(0);
                break;
        }
    }

    return 0;
}