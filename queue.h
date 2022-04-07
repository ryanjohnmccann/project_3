#ifndef PROJECT_3_QUEUE_H
#define PROJECT_3_QUEUE_H

// Source for queue code: https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/

// TODO: Queue should have the ability to grow and shrink? No bounds?
struct Queue {
    int front, rear, size, capacity;
    int *array;
};

struct Queue *create_queue(int capacity);

int is_full(struct Queue *queue);

int is_empty(struct Queue *queue);

void enqueue(struct Queue *queue, int item);

int dequeue(struct Queue *queue);

int front(struct Queue *queue);

int rear(struct Queue *queue);

void print_queue(struct Queue *queue);

void clean_queue(struct Queue *queue);

#endif
