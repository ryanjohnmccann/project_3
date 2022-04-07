
#include <stdlib.h>
#include <stdio.h>

#include "sched_sim.h"
#include "queue.h"

struct CPUInfo cpu_info;
struct MethodStats method_stats[4];

extern struct Queue *arrival_queue;
extern struct Queue *ready_queue;
extern struct Queue *finished_queue;
extern struct ProcessInfo process_info[100];

void init_sim(int method_num) {
    cpu_info.time = 0;
    cpu_info.burst = -1;
    cpu_info.active_process = -1;
    cpu_info.state = 'N';

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void print_cpu(int time, char state, int pid, int second_pid, int burst, struct Queue *queue) {
    printf("t = %i\n", time);
    // Loading
    if (state == 'L') {
        printf("CPU: Loading process %i (CPU burst = %i)\n", pid, burst);
    }
    // Running
    else if (state == 'R') {
        printf("CPU: Running process %i (remaining CPU burst = %i)\n", pid, burst);
    }
    // Finishing
    else if (state == 'F') {
        printf("CPU: Finishing process %i; loading process %i (CPU burst = %i)\n", pid, second_pid, burst);
    }
    // TODO: Implement (preemptive)
    else {
        printf("=== IMPLEMENT ===\n");
    }
    print_queue(queue);
    printf("\n\n");
}

void fcfs(int snapshot) {
    int tmp = 0;

    printf("***** FCFS Scheduling *****\n");
    while (1) {
        // Add by pid since file is organized by arrival time
        while (1) {
            // Add to ready queue, remove from arrival
            if (process_info[rear(arrival_queue)].arrival >= cpu_info.time) {
                enqueue(ready_queue, process_info[rear(arrival_queue)].pid);
                dequeue(arrival_queue);
            }
            // Nothing else to add
            else {
                break;
            }
        }
        // TODO: Implement
        if (cpu_info.time == snapshot || (snapshot == 1 && cpu_info.time == 0)) {
        }
        break;
    }
}
