
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
    }

    cpu_info.time = 0;
    method_stats[method_num].context_switches = 0;
    // Invalid state temporarily
    cpu_info.state = 'N';
    cpu_info.snapshot = snapshot;
    load_pid = run_pid = -1;
    finished = 0;

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void fcfs(const char *output_file) {
    FILE *fp;

    fp = fopen(output_file, "a");
    fprintf(fp, "***** FCFS Scheduling *****\n");
    fclose(fp);
    while (!finished) {
        handle_arrival_queue(0, 'n');

        finished = handle_cycle(0, output_file);

        // Check if finished
        if (finished) {
            fp = fopen(output_file, "a");
            fprintf(fp, "*********************************************************\n");
            fprintf(fp, "FCFS Summary (WT = wait time, TT = turnaround time):\n\n");
            fclose(fp);
            write_summary(0, output_file);
        }
    }
}

void sjf(const char *output_file) {
    FILE *fp;

    fp = fopen(output_file, "a");
    fprintf(fp, "***** SJF Scheduling *****\n");
    fclose(fp);
    while (!finished) {
        handle_arrival_queue(1, 'b');

        finished = handle_cycle(1, output_file);

        // Check if finished
        if (finished) {
            fp = fopen(output_file, "a");
            fprintf(fp,"*********************************************************\n");
            fprintf(fp, "SJF Summary (WT = wait time, TT = turnaround time):\n\n");
            fclose(fp);
            write_summary(1, output_file);
        }
    }
}

void priority(const char *output_file) {
    FILE *fp;

    fp = fopen(output_file, "a");
    fprintf(fp, "***** Priority Scheduling *****\n");
    fclose(fp);
    while (!finished) {
        handle_arrival_queue(1, 'p');

        finished = handle_cycle(4, output_file);

        // Check if finished
        if (finished) {
            fp = fopen(output_file, "a");
            fprintf(fp, "*********************************************************\n");
            fprintf(fp, "Priority Summary (WT = wait time, TT = turnaround time):\n\n");
            fclose(fp);
            write_summary(4, output_file);
        }
    }
}

void stcf(const char *output_file) {
    FILE *fp;

    fp = fopen(output_file, "a");
    fprintf(fp, "***** STCF Scheduling *****\n");
    fclose(fp);
    while (!finished) {
        handle_arrival_queue(1, 'b');

        // Add currently running process to queue and sort again, take the shortest burst time
        if (run_pid != -1 && !is_empty(ready_queue) &&
            process_info[run_pid].burst > process_info[ready_queue->front->key].burst) {

            process_info[run_pid].burst -= 1;
            if (process_info[run_pid].burst == 0) {
                cpu_info.state = 'F';
            } else {
                old_pid = run_pid;
                run_pid = -1;
                cpu_info.state = 'P';
            }

        }
            // Preempt a loading process
        else if (load_pid != -1 && !is_empty(ready_queue) &&
                 (process_info[load_pid].burst - 1) > process_info[ready_queue->front->key].burst) {

            process_info[load_pid].burst -= 1;
            old_pid = load_pid;
            cpu_info.state = 'P';
            load_pid = -1;
        }

        finished = handle_cycle(2, output_file);
        // Handles the aftermath of a preemptive cycle, handle_cycle does not have functionality for that (yet)
        if (cpu_info.state == 'P') {
            enqueue(sequence_queue, old_pid);
            process_info[old_pid].wait += 1;
            method_stats[2].context_switches += 1;
            enqueue(ready_queue, old_pid);
            sort_queue(ready_queue, 'b');
            cpu_info.state = 'L';
        }

        // Check if finished
        if (finished) {
            fp = fopen(output_file, "a");
            fprintf(fp, "*********************************************************\n");
            fprintf(fp, "STCF Summary (WT = wait time, TT = turnaround time):\n\n");
            fclose(fp);
            write_summary(2, output_file);
        }
    }
}

void round_robin(const char *output_file) {
    FILE *fp;

    fp = fopen(output_file, "a");
    fprintf(fp, "***** Round robin Scheduling *****\n");
    fclose(fp);
    while (!finished) {
        handle_arrival_queue(0, 'n');

        if (run_pid != -1 && !is_empty(ready_queue)) {
            process_info[run_pid].burst -= 1;
            if (process_info[run_pid].burst == 0) {
                cpu_info.state = 'F';
            } else {
                old_pid = run_pid;
                run_pid = -1;
                cpu_info.state = 'P';
            }
        }

        finished = handle_cycle(2, output_file);
        if (cpu_info.state == 'P') {
            enqueue(sequence_queue, old_pid);
            process_info[old_pid].wait += 1;
            method_stats[3].context_switches += 1;
            enqueue(ready_queue, old_pid);
            cpu_info.state = 'L';
        }

        // Check if finished
        if (finished) {
            fp = fopen(output_file, "a");
            fprintf(fp, "*********************************************************\n");
            fprintf(fp, "Round robin Summary (WT = wait time, TT = turnaround time):\n\n");
            fclose(fp);
            write_summary(3, output_file);
        }
    }
}
