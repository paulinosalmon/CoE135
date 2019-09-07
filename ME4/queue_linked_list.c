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
        printf("%d\t", temp->data);
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
        printf("Underflow\n");
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

    node *a, *b, *c;
    a = new_node(10);
    b = new_node(20);
    c = new_node(30);

    dequeue(q);
    enqueue(q, a);
    enqueue(q, b);
    enqueue(q, c);

    traversal(q);
    dequeue(q);
    traversal(q);


    return 0;
}