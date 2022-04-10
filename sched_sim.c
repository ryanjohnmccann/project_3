// TODO: Update doc strings
// TODO: Change print to write to output file
// TODO: Export helpful functions to different file?

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
    method_stats[method_num].context_switches = 0;
    // Invalid state
    cpu_info.state = 'N';

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void print_cpu(float pid, float second_pid, float burst) {
    printf("t = %i\n", cpu_info.time);
    // Loading
    if (cpu_info.state == 'L') {
        printf("CPU: Loading process %.0f (CPU burst = %.0f)\n", pid, burst);
    }
        // Running
    else if (cpu_info.state == 'R') {
        printf("CPU: Running process %.0f (remaining CPU burst = %.0f)\n", pid, burst);
    }
        // Finishing and loading
    else if (cpu_info.state == 'B') {
        printf("CPU: Finishing process %.0f; loading process %.0f (CPU burst = %.0f)\n", pid, second_pid, burst);
    }
        // Finishing
    else if (cpu_info.state == 'F') {
        printf("CPU: Finishing process %.0f\n", pid);
    }
        // TODO: Implement (preemptive)
    else {
        printf("=== IMPLEMENT ===\n");
    }
    printf("Ready queue: ");
    print_queue(ready_queue);
    printf("\n\n");
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

void print_summary(int method_num) {
    float process_count = 0;
    printf("PID\t\tWT\t\tTT\n");
    for (int i = front(finished_queue); i <= rear(finished_queue); i++) {
        printf("%.0f\t\t%.0f\t\t%.0f\n", process_info[i].pid, process_info[i].wait,
               (process_info[i].finished + process_info[i].arrival));
        method_stats[method_num].avg_wt += process_info[i].wait;
        // TODO: Only for non-preemptive
        method_stats[method_num].avg_tt += (process_info[i].finished - process_info[i].arrival);
        process_count += 1;
    }
    method_stats[method_num].avg_wt /= process_count;
    method_stats[method_num].avg_tt /= process_count;
    printf("AVG\t\t%.2f\t%.2f\n\n", method_stats[method_num].avg_wt, method_stats[method_num].avg_tt);
    printf("Process sequence: ");
    print_queue(finished_queue);
    printf("\n");
    printf("Context switches %i\n", method_stats[method_num].context_switches);
}

void handle_finished_process() {
    if (process_info[run_pid].burst <= 0) {
        process_info[run_pid].finished = cpu_info.time;
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

void calculate_wait() {
    int i;
    if (!is_empty(ready_queue)) {
        for (i = front(ready_queue); i <= rear(ready_queue); i++) {
            // Do not count a loading process as waiting
            if (i != load_pid) {
                process_info[i].wait += 1;
            }
        }
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
