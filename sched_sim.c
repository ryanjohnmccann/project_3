// TODO: Update doc strings
// TODO: Change print to write to output file

// Standard imports
#include <stdio.h>

// Header file imports
#include "sched_sim.h"
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
    cpu_info.burst = -1;
    cpu_info.active_process = -1;
    // Invalid state
    cpu_info.state = 'N';

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void print_cpu(int pid, int second_pid, int burst) {
    printf("t = %i\n", cpu_info.time);
    // Loading
    if (cpu_info.state == 'L') {
        printf("CPU: Loading process %i (CPU burst = %i)\n", pid, burst);
    }
        // Running
    else if (cpu_info.state == 'R') {
        printf("CPU: Running process %i (remaining CPU burst = %i)\n", pid, burst);
    }
        // Finishing and loading
    else if (cpu_info.state == 'B') {
        printf("CPU: Finishing process %i; loading process %i (CPU burst = %i)\n", pid, second_pid, burst);
    }
        // Finishing
    else if (cpu_info.state == 'F') {
        printf("CPU: Finishing process %i\n", pid);
    }
        // TODO: Implement (preemptive)
    else {
        printf("=== IMPLEMENT ===\n");
    }
    printf("Ready queue: ");
    print_queue(ready_queue);
    printf("\n\n");
}

void print_summary() {

}

void handle_cpu_print() {
    if ((cpu_info.time % cpu_info.snapshot) == 0 || (cpu_info.snapshot == 1 && cpu_info.time == 0)) {
        // Loading
        if (cpu_info.state == 'L') {
            print_cpu(load_pid, -1, process_info[load_pid].burst);
        }
            // Running or just finishing (with no load)
        else if (cpu_info.state == 'R' || cpu_info.state == 'F') {
            print_cpu(run_pid, -1, process_info[run_pid].burst);
        }
            // Finishing and loading
        else if (cpu_info.state == 'B') {
            print_cpu(run_pid, load_pid, process_info[load_pid].burst);
        }
    }
}

void handle_finished_process() {
    if (process_info[run_pid].burst <= 0) {
        // Load the next process while finishing
        if (!is_empty(ready_queue)) {
            cpu_info.state = 'B';
            load_pid = dequeue(ready_queue);
        } else {
            cpu_info.state = 'F';
        }
        enqueue(finished_queue, run_pid);
    }
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
            process_info[run_pid].burst -= 1;
            handle_finished_process();
        }

        handle_cpu_print();

        // Finished running a process in this cycle, reset run_pid
        if (cpu_info.state == 'F' || cpu_info.state == 'B') {
            run_pid = -1;
        }

        cpu_info.time += 1;

        // Check if finished
        if (is_empty(arrival_queue) && is_empty(ready_queue) && load_pid == -1 && run_pid == -1) {
            print_summary();
            finished = 1;
        }
    }
}
