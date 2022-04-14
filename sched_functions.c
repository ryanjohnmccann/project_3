
// Standard imports
#include <stdio.h>
#include <stdlib.h>

// Header file imports
#include "queue.h"
#include "sched_functions.h"
#include "sched_sim.h"

// Global variables
extern struct CPUInfo cpu_info;
extern struct MethodStats method_stats[4];

extern struct Queue *arrival_queue;
extern struct Queue *ready_queue;
extern struct Queue *sequence_queue;
extern struct ProcessInfo process_info[100];

extern int run_pid, load_pid, old_pid;
extern int arr_len;

void write_cpu_cycle(float pid, float second_pid, float burst, float second_burst, const char *file_name) {
    FILE *fp;

    fp = fopen(file_name, "a");


    fprintf(fp, "t = %i\n", cpu_info.time);
    // Loading
    if (cpu_info.state == 'L') {
        fprintf(fp, "CPU: Loading process %.0f (CPU burst = %.0f)\n", pid, burst);
    }
        // Running
    else if (cpu_info.state == 'R') {
        fprintf(fp, "CPU: Running process %.0f (remaining CPU burst = %.0f)\n", pid, burst);
    }
        // Finishing and loading
    else if (cpu_info.state == 'B') {
        fprintf(fp, "CPU: Finishing process %.0f; loading process %.0f (CPU burst = %.0f)\n", pid, second_pid, burst);
    }
        // Finishing
    else if (cpu_info.state == 'F') {
        fprintf(fp, "CPU: Finishing process %.0f\n", pid);
    }
        // Preempting a process
    else if (cpu_info.state == 'P') {
        fprintf(fp, "CPU: Preempting process %.0f (remaining CPU burst = %.0f); loading process %.0f"
                    " (CPU burst = %.0f)\n", pid, burst, second_pid, second_burst);
    }
    fprintf(fp, "Ready queue: ");
    print_queue(ready_queue, fp);
    fprintf(fp, "\n\n");
    fclose(fp);
}

void handle_write_cpu_cycle(const char *file_name) {
    if ((cpu_info.time % cpu_info.snapshot) == 0 || (cpu_info.snapshot == 1 && cpu_info.time == 0)) {
        // Loading
        if (cpu_info.state == 'L') {
            write_cpu_cycle(load_pid, -1, process_info[load_pid].burst, 0, file_name);
        }
            // Running or just finishing (with no load)
        else if (cpu_info.state == 'R' || cpu_info.state == 'F') {
            write_cpu_cycle(run_pid, -1, process_info[run_pid].burst, 0, file_name);
        }
            // Finishing and loading
        else if (cpu_info.state == 'B') {
            write_cpu_cycle(run_pid, load_pid, process_info[load_pid].burst, 0, file_name);
        } else if (cpu_info.state == 'P') {
            write_cpu_cycle(old_pid, load_pid, process_info[old_pid].burst,
                            process_info[load_pid].burst, file_name);
        }
    }
}

void write_summary(int method_num, const char *file_name) {
    FILE *fp;

    fp = fopen(file_name, "a");

    fprintf(fp, "PID\t\tWT\t\tTT\n");

    for (int i = 0; i < arr_len; i++) {
        fprintf(fp, "%.0f\t\t%.0f\t\t%.0f\n", process_info[i].pid, process_info[i].wait,
                (process_info[i].finished - process_info[i].arrival));
        method_stats[method_num].avg_wt += process_info[i].wait;
        method_stats[method_num].avg_tt += (process_info[i].finished - process_info[i].arrival);
    }

    method_stats[method_num].context_switches = get_size(sequence_queue);
    method_stats[method_num].avg_wt /= ((float) arr_len);
    method_stats[method_num].avg_tt /= ((float) arr_len);

    fprintf(fp, "AVG\t\t%.2f\t%.2f\n\n", method_stats[method_num].avg_wt, method_stats[method_num].avg_tt);
    fprintf(fp, "Process sequence: ");
    print_queue(sequence_queue, fp);
    fprintf(fp, "\n");
    fprintf(fp, "Context switches: %i\n\n", method_stats[method_num].context_switches);
    fclose(fp);
}

/***
 * Determines if the currently running process is about to finish or not. If true, function handles the next cpu state
 * and potentially a new loading process
 */
void handle_finished_process() {
    // Technically this number should never be less than zero
    if (process_info[run_pid].burst <= 0) {
        process_info[run_pid].finished = cpu_info.time;
        // Load the next process while finishing
        if (!is_empty(ready_queue)) {
            cpu_info.state = 'B';
            load_pid = ready_queue->front->key;
        } else {
            cpu_info.state = 'F';
        }
        enqueue(sequence_queue, run_pid);
    }
}

/***
 * Traverses through the ready queue and updates wait times for all processes in it
 */
void calculate_wait() {
    int size = 0;
    int tmp_arr[get_size(ready_queue)];

    while (!is_empty(ready_queue)) {
        tmp_arr[size] = ready_queue->front->key;
        dequeue(ready_queue);
        size += 1;
    }

    for (int i = 0; i < size; i++) {
        // Do not add to wait time for a loading process
        if (process_info[tmp_arr[i]].pid != load_pid) {
            process_info[tmp_arr[i]].wait += 1;
        }
        enqueue(ready_queue, tmp_arr[i]);
    }
}

/***
 * Runs a cycle of the CPU
 * @param method_num A number to determine which scheduling algorithm is currently running
 * @return 1 if last cycle, 0 otherwise
 */
int handle_cycle(int method_num, const char *file_name) {
    // Last cycle was a loading state, process is ready to be removed from the ready queue
    if (cpu_info.state == 'L') {
        dequeue(ready_queue);
    }

    // Running a process
    if (run_pid != -1) {
        cpu_info.state = 'R';
        process_info[run_pid].burst -= 1;
        // Process may or may not be finished, function determines if still running and if not will handle it
        handle_finished_process();
    }
        // Not running and something in the ready queue, load a new process
    else if (!is_empty(ready_queue) && load_pid == -1) {
        load_pid = ready_queue->front->key;
        if (cpu_info.state != 'P') {
            cpu_info.state = 'L';
        }
    }
        // Loaded a new process in the prior cycle and nothing running now, start running
    else if (load_pid != -1) {
        run_pid = load_pid;
        load_pid = -1;
        cpu_info.state = 'R';
        process_info[run_pid].burst -= 1;
        // Process may or may not be finished, function determines if still running and if not will handle it
        handle_finished_process();
    }

    // Determines if we need to print cpu contents
    handle_write_cpu_cycle(file_name);

    // Don't dequeue from the ready queue until after we print
    if (cpu_info.state == 'B') {
        dequeue(ready_queue);
    }

    // Update wait times (if needed) each cycle
    calculate_wait();

    // Finished running a process in this cycle, reset run_pid
    if (cpu_info.state == 'F' || cpu_info.state == 'B') {
        run_pid = -1;
    }

    cpu_info.time += 1;
    if (is_empty(arrival_queue) && is_empty(ready_queue) && load_pid == -1 && run_pid == -1) {
        return 1;
    }
    return 0;
}

/***
 * Determines if something from the arrival queue should be added to the ready queue, sorts the ready queue afterwards
 * if necessary
 * @param will_sort To determine if we need to sort e.g. FCFS does not
 * @param sort_by Determines what we sort by e.g. burst or priority
 */
void handle_arrival_queue(int will_sort, char sort_by) {
    while (!is_empty(arrival_queue) && process_info[arrival_queue->front->key].arrival <= cpu_info.time) {

        enqueue(ready_queue, process_info[arrival_queue->front->key].pid);
        dequeue(arrival_queue);

        if (get_size(ready_queue) > 1 && will_sort) {
            sort_queue(ready_queue, sort_by);
        }
    }
}
