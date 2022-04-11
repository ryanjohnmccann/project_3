
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "queue.h"
#include "sched_sim.h"

// Global variables
extern struct ProcessInfo process_info[100];

// Source for Queue code: https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/

struct Queue *create_queue(int capacity) {
    struct Queue *queue = (struct Queue *) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;

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

void print_queue(struct Queue *queue) {
    int i;
    if (queue->size == 0) {
        printf("empty");
    } else {
        for (i = queue->front; i <= queue->rear; i++) {
            printf("%i", queue->array[i]);
            if (i != queue->rear && queue->size > 1) {
                printf("-");
            }
        }
    }
}

int find_min(struct Queue *queue, char method, int sorted_index) {
    int min_index = -1;
    int min_val = INT_MAX;
    for (int i = 0; i < queue->size; i++) {
        int curr_pid = front(queue);
        dequeue(queue);

        if (method == 'b') {
            if (process_info[curr_pid].burst < min_val && i <= sorted_index) {
                min_index = i;
                min_val = curr_pid;
            }
        }
        enqueue(queue, curr_pid);

    }
    return min_index;
}

void insert_min_to_rear(struct Queue *queue, int min_index) {
    int min_val;
    for (int i = 0; i < queue->size; i++) {
        int curr_pid = dequeue(queue);
        if (i != min_index) {
            enqueue(queue, curr_pid);
        } else {
            min_val = curr_pid;
        }
    }
    enqueue(queue, min_val);
}

// TODO: Ask the professor if I can use this code
// Code has been modified from source: https://www.geeksforgeeks.org/sorting-queue-without-extra-space/
void sort_queue(struct Queue *queue, char method) {
    for (int i = 1; i <= queue->size; i++) {
        // Finds the pid with the minimum value
        int min = find_min(queue, method, queue->size - i);
        // Add to the back of the queue
        insert_min_to_rear(queue, min);
    }
}
