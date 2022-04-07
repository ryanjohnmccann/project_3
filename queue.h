#ifndef PROJECT_3_QUEUE_H
#define PROJECT_3_QUEUE_H

// TODO: Queue should have the ability to grow and shrink? No bounds?
struct Queue {
    int front, rear;
    int *array;
};

void create_queue(struct Queue *queue);

void is_empty(struct Queue *queue);

void enqueue(struct Queue *queue, int item);

int dequeue(struct Queue *queue);

void print_queue(struct Queue *queue);

void clean_queue(struct Queue *queue);

#endif //PROJECT_3_QUEUE_H
