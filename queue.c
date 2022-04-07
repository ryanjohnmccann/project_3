
#include <stdlib.h>
#include <limits.h>

#include "queue.h"

// Source for Queue code: https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/

struct Queue *create_queue(int capacity) {
    struct Queue *queue = (struct Queue *) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->rear = capacity - 1;
    queue->array = (int *) malloc(queue->capacity * sizeof(int));
    return queue;
}

int is_full(struct Queue *queue) {
    return (queue->size == queue->capacity);
}

int is_empty(struct Queue *queue) {
    return (queue->size == 0);
}

void enqueue(struct Queue *queue, int item) {
    if (is_full(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(struct Queue *queue) {
    if (is_empty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(struct Queue *queue) {
    if (is_empty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

int rear(struct Queue *queue) {
    if (is_empty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

// TODO: Implement
void clean_queue(struct Queue *queue) {

}

// TODO: Implement
void print_queue(struct Queue *queue) {

}
