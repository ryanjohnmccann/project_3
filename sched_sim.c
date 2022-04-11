// TODO: Update doc strings
// TODO: Change print to write to output file

// Standard imports
#include <stdio.h>

// Header file imports
#include "sched_sim.h"
#include "sched_functions.h"
#include "queue.h"

// Global variables
struct CPUInfo cpu_info;
struct MethodStats method_stats[4];

extern struct Queue *arrival_queue;
extern struct Queue *ready_queue;
extern struct Queue *finished_queue;
extern struct ProcessInfo process_info[100];

extern int snapshot;

int run_pid, load_pid, finished;

void init_sim(int method_num) {
    cpu_info.time = 0;
    method_stats[method_num].context_switches = 0;
    // Invalid state
    cpu_info.state = 'N';
    cpu_info.snapshot = snapshot;
    load_pid = run_pid = -1;
    finished = 0;

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void fcfs() {
    printf("***** FCFS Scheduling *****\n");
    while (!finished) {
        // Add by pid since file is organized by arrival time
        while (!is_empty(arrival_queue) && process_info[front(arrival_queue)].arrival <= cpu_info.time) {
            // Add to ready queue, remove from arrival
            enqueue(ready_queue, process_info[front(arrival_queue)].pid);
            dequeue(arrival_queue);
        }

        finished = handle_nonpre_cycle();

        // Check if finished
        if (finished) {
            printf("*********************************************************\n");
            printf("FCFS Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(0);
        }
    }
}

// TODO: How to break a tie between same arrival and same burst time?
void sjf() {
    printf("***** SJF Scheduling *****\n");
    while (!finished) {
        // Add by pid since file is organized by arrival time
        while (!is_empty(arrival_queue) && process_info[front(arrival_queue)].arrival <= cpu_info.time) {

            // Rearrange ready queue by burst time
            if (!is_empty(ready_queue)) {
                sort_queue(ready_queue, 'b');
            }
            enqueue(ready_queue, process_info[front(arrival_queue)].pid);
            dequeue(arrival_queue);
        }

        finished = handle_nonpre_cycle();

        // Check if finished
        if (finished) {
            printf("*********************************************************\n");
            printf("SJF Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(1);
        }
    }
}

