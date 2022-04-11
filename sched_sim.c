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
extern int arr_len;

int run_pid, load_pid, finished;

void init_sim(int method_num) {
    // Flush all prior contents and free allocated memory
    clean_queue(ready_queue);
    clean_queue(finished_queue);

    ready_queue = create_queue(arr_len);
    finished_queue = create_queue(arr_len);

    for (int i = 0; i < arr_len; i++) {
        process_info[i].wait = 0;
        process_info[i].finished = 0;
        process_info[i].turn_time = 0;
    }

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

            enqueue(ready_queue, process_info[front(arrival_queue)].pid);
            dequeue(arrival_queue);
            // Rearrange ready queue by burst time
            if (ready_queue->size > 1) {
                sort_queue(ready_queue, 'b');
            }
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

void priority() {
    printf("***** Priority Scheduling *****\n");
    while (!finished) {
        // Add by pid since file is organized by arrival time
        while (!is_empty(arrival_queue) && process_info[front(arrival_queue)].arrival <= cpu_info.time) {

            enqueue(ready_queue, process_info[front(arrival_queue)].pid);
            dequeue(arrival_queue);
            // Rearrange ready queue by burst time
            if (ready_queue->size > 1) {
                sort_queue(ready_queue, 'p');
            }
        }

        finished = handle_nonpre_cycle();

        // Check if finished
        if (finished) {
            printf("*********************************************************\n");
            printf("Priority Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(4);
        }
    }
}

// TODO: Create non-pre sim function to neaten up this code

