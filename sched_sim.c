
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
        handle_arrival_queue(0, 'n');

        finished = handle_nonpre_cycle(0);

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

        finished = handle_nonpre_cycle(1);

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

        finished = handle_nonpre_cycle(4);

        // Check if finished
        if (finished) {
            printf("*********************************************************\n");
            printf("Priority Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(4);
        }
    }
}

void stcf() {
    printf("***** STCF Scheduling *****\n");
     while (!finished) {
         handle_arrival_queue(1, 'b');

         // Add currently running process to queue and sort again, take the shortest burst time
         // TODO: Possible that we're loading and running?
         if (run_pid != -1) {
             int old_run = run_pid;
             enqueue(ready_queue, run_pid);
             sort_queue(ready_queue, 'b');
             run_pid = dequeue(ready_queue);
             // Preempted, load another process
             if (old_run != run_pid) {

             }
         }
         else if (load_pid != -1) {
             int old_load = load_pid;
             enqueue(ready_queue, run_pid);
             sort_queue(ready_queue, 'b');
             load_pid = dequeue(ready_queue);
             // Preempt a process after just one cycle
             if (old_load != load_pid) {

             }
         }

         finished = handle_nonpre_cycle(2);

         // Check if finished
         if (finished) {
             printf("*********************************************************\n");
             printf("STCF Summary (WT = wait time, TT = turnaround time):\n\n");
             print_summary(2);
         }
     }
}

// TODO: Create non-pre sim function to neaten up this code

