
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sched_sim.h"

extern struct ProcessInfo process_info[100];

// Source for queue code: https://www.geeksforgeeks.org/queue-linked-list-implementation/

// A utility function to create a new linked list node.
struct QNode *new_node(int k) {
    struct QNode *temp = (struct QNode *) malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue *create_queue() {
    struct Queue *q = (struct Queue *) malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// The function to add a key k to q
void enqueue(struct Queue *q, int k) {
    // Create a new LL node
    struct QNode *temp = new_node(k);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove a key from given queue q
void dequeue(struct Queue *q) {
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return;

    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;

    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

void clean_queue(struct Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
}

int is_empty(struct Queue *q) {
    if (q->front == NULL && q->rear == NULL) {
        return 1;
    }
    return 0;
}

void print_queue(struct Queue *q) {
    struct QNode *tmp_node = q->front;
    if (tmp_node == NULL) {
        printf("empty");
    } else {
        while (tmp_node != NULL) {
            printf("%i", tmp_node->key);
            tmp_node = tmp_node->next;
            if (tmp_node != NULL) {
                printf("-");
            }
        }
    }
}

int get_size(struct Queue *q) {
    int size = 0;
    struct QNode *tmp_node = q->front;
    while (tmp_node != NULL) {
        size += 1;
        tmp_node = tmp_node->next;
    }
    return size;
}

void transfer_queue_arr(struct Queue *q, int *arr) {
    int index = 0;
    // Add all elements to an array (dumb I know)
    while (!is_empty(q)) {
        arr[index] = q->front->key;
        index += 1;
        dequeue(q);
    }
}

void transfer_arr_queue(struct Queue *q, int *arr, int size) {
    // Add items back to queue
    for (int i = 0; i < size; i++) {
        enqueue(q, arr[i]);
    }
}

// Code has been modified from source: https://www.sanfoundry.com/c-program-sort-array-descending-order/
void sort_queue(struct Queue *q, char method) {
    int tmp, swap;

    int size = get_size(q);
    int tmp_arr[size];

    transfer_queue_arr(q, tmp_arr);

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

    transfer_arr_queue(q, tmp_arr, size);

}
