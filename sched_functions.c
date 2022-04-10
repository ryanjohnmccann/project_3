
// Standard imports
#include <stdio.h>

// Header file imports
#include "queue.h"
#include "sched_functions.h"
#include "sched_sim.h"

// Global variables
extern struct CPUInfo cpu_info;
extern struct MethodStats method_stats[4];

extern struct Queue *arrival_queue;
extern struct Queue *ready_queue;
extern struct Queue *finished_queue;
extern struct ProcessInfo process_info[100];

extern int run_pid, load_pid;

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
