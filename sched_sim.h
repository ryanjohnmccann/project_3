#ifndef PROJECT_3_SCHED_SIM_H
#define PROJECT_3_SCHED_SIM_H

// TODO: Move queue to separate files


struct CPUInfo {
    int burst, time, active_process;
    char state[50];
    // TODO: Past run processes (ordered list)
};

struct ProcessInfo {
    int pid, burst, priority, arrival, wait, turn_time;
    // TODO: Other helper data?
};

struct MethodStats {
    float avg_wt, avg_tt;
    int context_switches;
};

// TODO: Queue should have the ability to grow and shrink? No bounds?
struct Queue {
    int front, rear, size;
    unsigned capacity;
    int *array;
};

struct Queue *create_queue(unsigned capacity);

int is_full(struct Queue *queue);

int is_empty(struct Queue *queue);

void enqueue(struct Queue *queue, int item);

int dequeue(struct Queue *queue);

void print_queue(struct Queue *queue);

void clean_queue(struct Queue *);

#endif //PROJECT_3_SCHED_SIM_H
