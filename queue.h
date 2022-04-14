#ifndef PROJECT_3_QUEUE_H
#define PROJECT_3_QUEUE_H

// Source for queue code: https://www.geeksforgeeks.org/queue-linked-list-implementation/

// A linked list (LL) node to store a queue entry
struct QNode {
    int key;
    struct QNode *next;
};
// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
    struct QNode *front, *rear;
};

struct QNode *new_node(int k);

struct Queue *create_queue();

void enqueue(struct Queue *q, int k);

void dequeue(struct Queue *q);

void clean_queue(struct Queue *q);

int is_empty(struct Queue *q);

void print_queue(struct Queue *q, FILE *fp);

int get_size(struct Queue *q);

void sort_queue(struct Queue *q, char method);

#endif
