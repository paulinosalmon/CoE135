#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} node;

typedef struct linked_list {
    struct node *head;
    struct node *tail;
} queue;

node* new_node(int data) {
    node *z;
    z = malloc(sizeof(struct node));
    z->data = data;
    z->next = NULL;

    return z;
}

queue* new_queue() {
    queue *q = malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;

    return q;
}

void traversal(queue *q) {
    node *temp = q->head; 

    while(temp != NULL) { 
        printf("%d ", temp->data);
        temp = temp->next;
    }

    printf("\n");
}

int is_empty(queue *q) {
    if(q->head == NULL)
        return 1;
    return 0;
}

void enqueue(queue *q, node *n) {
    if(is_empty(q)) {
        q->head = n;
        q->tail = n;
    }
    else {
        q->tail->next = n;
        q->tail = n;
    }
}

int dequeue(queue *q) {
    if(is_empty(q)) {
        traversal(q);
        return -1000;
    }
    else {
        int x = q->head->data;
        node *temp = q->head;
        q->head = q->head->next;
        free(temp);
        return x;
    }
}

int main() {
    queue *q = new_queue();
    int choice;

    while(1) {
        node* a;
        int input;
        printf("\nWhat do you want to do?\n1) Insert integer.\n2) Pop integer.\n3) Exit\n");
        scanf("%d", &choice);
        switch(choice) {
            case 1: 
                printf("Enter integer to insert: ");
                scanf("%d", &input);
                a = new_node(input);
                enqueue(q, a);
                printf("Inserted. Queue is now: \n");
                traversal(q);
                break;
            case 2:
                printf("Popping first in queue...\n");
                dequeue(q);
                printf("Popped. Queue is now:\n");
                traversal(q);
                break;
            default:
                exit(0);
                break;
        }
    }

    return 0;
}