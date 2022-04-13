// TODO: There is a context switch bug\
// TODO: Transfer prints to write to file

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

extern int run_pid, load_pid, old_pid;
extern int arr_len;

void print_cpu(float pid, float second_pid, float burst, float second_burst) {
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
        // Preempting a process
    else if (cpu_info.state == 'P') {
        printf("CPU: Preempting process %.0f (remaining CPU burst = %.0f); loading process %.0f"
               " (CPU burst = %.0f)\n", pid, burst, second_pid, second_burst);
    }
    printf("Ready queue: ");
    print_queue(ready_queue);
    printf("\n\n");
}

void handle_cpu_print() {
    if ((cpu_info.time % cpu_info.snapshot) == 0 || (cpu_info.snapshot == 1 && cpu_info.time == 0)) {
        // Loading
        if (cpu_info.state == 'L') {
            print_cpu(load_pid, -1, process_info[load_pid].burst, 0);
        }
            // Running or just finishing (with no load)
        else if (cpu_info.state == 'R' || cpu_info.state == 'F') {
            print_cpu(run_pid, -1, process_info[run_pid].burst, 0);
        }
            // Finishing and loading
        else if (cpu_info.state == 'B') {
            print_cpu(run_pid, load_pid, process_info[load_pid].burst, 0);
        } else if (cpu_info.state == 'P') {
            print_cpu(old_pid, load_pid, process_info[old_pid].burst,
                      process_info[load_pid].burst);
        }
    }
}

void print_summary(int method_num) {
    printf("PID\t\tWT\t\tTT\n");

    for (int i = 0; i < arr_len; i++) {
        printf("%.0f\t\t%.0f\t\t%.0f\n", process_info[i].pid, process_info[i].wait,
               (process_info[i].finished - process_info[i].arrival));
        method_stats[method_num].avg_wt += process_info[i].wait;
        // TODO: Only for non-preemptive?
        method_stats[method_num].avg_tt += (process_info[i].finished - process_info[i].arrival);
    }

    method_stats[method_num].avg_wt /= ((float) arr_len);
    method_stats[method_num].avg_tt /= ((float) arr_len);

    printf("AVG\t\t%.2f\t%.2f\n\n", method_stats[method_num].avg_wt, method_stats[method_num].avg_tt);
    printf("Process sequence: ");
    print_queue(finished_queue);
    printf("\n");
    printf("Context switches: %i\n\n", method_stats[method_num].context_switches);
}

void handle_finished_process() {
    if (process_info[run_pid].burst <= 0) {
        process_info[run_pid].finished = cpu_info.time;
        // Load the next process while finishing
        if (!is_empty(ready_queue)) {
            cpu_info.state = 'B';
            load_pid = front(ready_queue);
        } else {
            cpu_info.state = 'F';
        }
        enqueue(finished_queue, run_pid);
    }
}

void calculate_wait() {
    int size = 0;
    int tmp_arr[ready_queue->size];

    while (!is_empty(ready_queue)) {
        tmp_arr[size] = dequeue(ready_queue);
        size += 1;
    }

    for (int i = 0; i < size; i++) {
        if (process_info[tmp_arr[i]].pid != load_pid) {
            process_info[tmp_arr[i]].wait += 1;
        }
        enqueue(ready_queue, tmp_arr[i]);
    }
}

int handle_nonpre_cycle(int method_num) {
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
        load_pid = front(ready_queue);
        if (cpu_info.state != 'P') {
            cpu_info.state = 'L';
        }
    }
        // Loaded a new process in the prior cycle and nothing running now, start running
    else if (load_pid != -1) {
        run_pid = load_pid;
        load_pid = -1;
        cpu_info.state = 'R';
        method_stats[method_num].context_switches += 1;
        process_info[run_pid].burst -= 1;
        // Process may or may not be finished, function determines if still running and if not will handle it
        handle_finished_process();
    }

    // Determines if we need to print cpu contents
    handle_cpu_print();

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

void handle_arrival_queue(int will_sort, char sort_by) {
    while (!is_empty(arrival_queue) && process_info[front(arrival_queue)].arrival <= cpu_info.time) {

        enqueue(ready_queue, process_info[front(arrival_queue)].pid);
        dequeue(arrival_queue);
        // Rearrange ready queue by burst time
        if (ready_queue->size > 1 && will_sort) {
            sort_queue(ready_queue, sort_by);
        }
    }
}
