// TODO: Update doc strings
// TODO: Change print to write to output file
// TODO: Export helpful functions to different file?

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

int run_pid, load_pid;

void init_sim(int method_num) {
    cpu_info.time = 0;
    method_stats[method_num].context_switches = 0;
    // Invalid state
    cpu_info.state = 'N';

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void fcfs(int snapshot) {
    int finished = 0;

    load_pid = -1;
    run_pid = -1;
    cpu_info.snapshot = snapshot;

    printf("***** FCFS Scheduling *****\n");
    while (!finished) {
        // Add by pid since file is organized by arrival time
        while (!is_empty(arrival_queue) && process_info[front(arrival_queue)].arrival <= cpu_info.time) {
            // Add to ready queue, remove from arrival
            enqueue(ready_queue, process_info[front(arrival_queue)].pid);
            dequeue(arrival_queue);
        }

        // Last cycle was a loading state, process is ready to be removed from the ready queue
        if (cpu_info.state == 'L') {
            dequeue(ready_queue);
        }

        // Running a process
        if (run_pid != -1) {
            cpu_info.state = 'R';
            process_info[run_pid].burst -= 1;
            handle_finished_process();
        }
            // Not running and something in the ready queue, load a new process
        else if (!is_empty(ready_queue) && load_pid == -1) {
            load_pid = front(ready_queue);
            cpu_info.state = 'L';
        }
            // Loaded a new process in the prior cycle and nothing running now, start running
        else if (load_pid != -1) {
            run_pid = load_pid;
            load_pid = -1;
            cpu_info.state = 'R';
            method_stats[0].context_switches += 1;
            process_info[run_pid].burst -= 1;
            handle_finished_process();
        }

        handle_cpu_print();

        calculate_wait();

        // Finished running a process in this cycle, reset run_pid
        if (cpu_info.state == 'F' || cpu_info.state == 'B') {
            run_pid = -1;
        }

        cpu_info.time += 1;

        // Check if finished
        if (is_empty(arrival_queue) && is_empty(ready_queue) && load_pid == -1 && run_pid == -1) {
            // Calculate turnaround times

            printf("*********************************************************\n");
            printf("FCFS Summary (WT = wait time, TT = turnaround time):\n\n");
            print_summary(0);
            finished = 1;
        }
    }
}

void sjf() {

}

