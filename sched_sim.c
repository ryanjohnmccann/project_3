
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
extern struct Queue *sequence_queue;
extern struct ProcessInfo process_info[10000];

extern int snapshot;
extern int arr_len;

int run_pid, load_pid, finished, old_pid;

/***
 * Initializes a simulation, prepares for a new scheduling algorithm to run
 * @param method_num An integer associated with each scheduling algorithm
 */
void init_sim(int method_num) {

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
        handle_arrival_queue(0, 'n');

        finished = handle_cycle(0);

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
        handle_arrival_queue(1, 'b');

        finished = handle_cycle(1);

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
        handle_arrival_queue(1, 'p');

        finished = handle_cycle(4);

        // Check if finished
        if (finished) {
            printf("*********************************************************\n");
            printf("Priority Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(4);
        }
    }
}

void stcf() {
    // TODO: Fix "finished" queue
    // TODO:
    printf("***** STCF Scheduling *****\n");
    while (!finished) {
        handle_arrival_queue(1, 'b');

        // Add currently running process to queue and sort again, take the shortest burst time
        // TODO: Possible that we're loading and running?
        // TODO: Fix this
        if (run_pid != -1) {
            if (!is_empty(ready_queue) &&
                process_info[run_pid].burst > process_info[ready_queue->front->key].burst) {

                old_pid = run_pid;
                run_pid = -1;
                cpu_info.state = 'P';
            }
        } else if (load_pid != -1) {
            // Preempt a loading process
            if (!is_empty(ready_queue) &&
                process_info[load_pid].burst - 1 > process_info[ready_queue->front->key].burst) {

                old_pid = load_pid;
                cpu_info.state = 'P';
                load_pid = -1;
            }
        }

        finished = handle_cycle(2);
        if (cpu_info.state == 'P') {
            // TODO: Not a good idea to have a bounded queue now!
            enqueue(sequence_queue, old_pid);
            process_info[old_pid].wait += 1;
            method_stats[2].context_switches += 1;
            process_info[old_pid].burst -= 1;
            enqueue(ready_queue, old_pid);
            sort_queue(ready_queue, 'b');
            cpu_info.state = 'L';
        }

        // Check if finished
        if (finished) {
            printf("*********************************************************\n");
            printf("STCF Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(2);
        }
    }
}

// TODO: Create non-pre sim function to neaten up this code

