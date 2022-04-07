#ifndef PROJECT_3_SCHED_SIM_H
#define PROJECT_3_SCHED_SIM_H


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

// TODO: Queue should have the ability to grow and shrink
// TODO: How to link process info struct here?
struct Queue {
    int front, rear;
    int *array;
};

void en_q(struct Queue *queue, int item);

void de_q(struct Queue *queue);

void print_q(struct Queue *queue);

void clean_q(struct Queue *);

#endif //PROJECT_3_SCHED_SIM_H
