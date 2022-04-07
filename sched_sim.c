
#include <stdlib.h>

#include "sched_sim.h"

struct Queue *create_queue(unsigned capacity) {
    struct Queue *queue = (struct Queue *) malloc(
            sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int *) malloc(
            queue->capacity * sizeof(int));
    return queue;
}
