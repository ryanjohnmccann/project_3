
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "queue.h"
#include "sched_sim.h"

// Global variables
extern struct ProcessInfo process_info[100];

extern int arr_len;

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

int transfer_queue_arr(struct Queue *queue, int *arr) {
    int size = 0;
    // Add all elements to an array (dumb I know)
    while (!is_empty(queue)) {
        arr[size] = dequeue(queue);
        size += 1;
    }
    return size;
}

void transfer_arr_queue(struct Queue *queue, int *arr, int size) {
    // Add items back to queue
    for (int i = 0; i < size; i++) {
        enqueue(queue, arr[i]);
    }
}

void print_queue(struct Queue *queue) {

    int tmp_arr[queue->size];
    int size = transfer_queue_arr(queue, tmp_arr);

    if (size == 0) {
        printf("empty");
    } else {
        for (int i = 0; i < size; i++) {
            printf("%i", tmp_arr[i]);
            if (size > 1 && i != size - 1) {
                printf("-");
            }
        }
    }
    transfer_arr_queue(queue, tmp_arr, size);
}

// TODO: Ask the professor if I can use this code
// Code has been modified from source: https://www.sanfoundry.com/c-program-sort-array-descending-order/
void sort_queue(struct Queue *queue, char method) {
    int tmp_arr[queue->size];
    int tmp, swap;

    int size = transfer_queue_arr(queue, tmp_arr);

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (method == 'b') {
                swap = process_info[tmp_arr[i]].burst > process_info[tmp_arr[j]].burst;
            } else if (method == 'p') {
                swap = process_info[tmp_arr[i]].priority > process_info[tmp_arr[j]].priority;
            }
            if (swap) {
                tmp = tmp_arr[i];
                tmp_arr[i] = tmp_arr[j];
                tmp_arr[j] = tmp;
            }
        }
    }

    transfer_arr_queue(queue, tmp_arr, size);

}
