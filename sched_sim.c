
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
        // Finishing and loading
    else if (state == 'FL') {
        printf("CPU: Finishing process %i; loading process %i (CPU burst = %i)\n", pid, second_pid, burst);
    }
        // Finishing
    else if (state == 'F') {

    }
        // TODO: Implement (preemptive)
    else {
        printf("=== IMPLEMENT ===\n");
    }
    printf("Ready queue: ");
    print_queue(queue);
    printf("\n\n");
}

void fcfs(int snapshot) {
    int load_pid = -1;
    int run_pid = -1;
    int finished = 0;

    printf("***** FCFS Scheduling *****\n");
    while (!finished) {
        // Add by pid since file is organized by arrival time
        while (process_info[rear(arrival_queue)].arrival >= cpu_info.time) {
            // Add to ready queue, remove from arrival
            enqueue(ready_queue, process_info[rear(arrival_queue)].pid);
            dequeue(arrival_queue);
        }

        // TODO: Possible for nothing to be in the ready queue?
        // TODO: Can we always display a loading state when finishing?
        // Currently, running a process
        if (run_pid != -1) {
            process_info[run_pid].burst -= 1;
            // Will finish this process in the next cycle, potentially load another process
            if (process_info[run_pid].burst == 0) {
                cpu_info.state = 'F';
                // Load the next process
                if (!is_empty(ready_queue)) {
                    load_pid = dequeue(ready_queue);
                }
                run_pid = -1;
                enqueue(finished_queue, run_pid);
            }
                // Continue running
            else {
                cpu_info.state = 'R';
            }
        }
            // Not running and something in the ready queue, load a new process
        else if (!is_empty(ready_queue) && load_pid == -1) {
            load_pid = dequeue(ready_queue);
            cpu_info.state = 'L';
        }
            // Loaded a new process and nothing running, start running
        else if (load_pid != -1) {
            run_pid = load_pid;
            load_pid = -1;
            cpu_info.state = 'R';
        }
        // Print
        if (cpu_info.time == snapshot || (snapshot == 1 && cpu_info.time == 0)) {
            if (cpu_info.state == 'L') {

            }
            print_cpu(cpu_info.time, cpu_info.state, run_pid, load_pid,
                      cpu_info.burst, ready_queue);
        }

        // TODO: Delete
        run_pid = 0;
        cpu_info.time += 1;
        finished = 1;
    }
}
