// TODO: Update doc strings
// TODO: Cite where you took code

// Standard imports
#include <stdio.h>
#include <stdlib.h>

// Header file imports
#include "sched_sim.h"
#include "queue.h"

// Global variables
struct Queue *arrival_queue;
struct Queue *ready_queue;
struct Queue *finished_queue;
// TODO: Find a better way to do this
struct ProcessInfo process_info[100];

void init_queues(const char *file_name) {
    // Flush all prior contents and free allocated memory
    clean_queue(arrival_queue);
    clean_queue(ready_queue);
    clean_queue(finished_queue);

    // Read file content and store into arrival queue
    FILE *fp;
    int arr_len = 0;
    int arr_index = 0;
    int pid = 0;
    int data_count = 1;
    int curr_num;

    fp = fopen(file_name, "r");
    while (!feof(fp)) {
        fscanf(fp, "%d", &curr_num);
        // Initialize new process
        if (data_count == 1) {
            process_info[arr_index].pid = pid;
            process_info[arr_index].burst = curr_num;
            process_info[arr_index].wait = 0;
            process_info[arr_index].turn_time = 0;
        } else if (data_count == 2) {
            process_info[arr_index].priority = curr_num;
        } else if (data_count == 3) {
            process_info[arr_index].arrival = curr_num;
            data_count = 0;
            arr_index += 1;
            pid += 1;
            arr_len += 1;
        }
        data_count += 1;
    }
    fclose(fp);

    ready_queue = create_queue(arr_len);
    finished_queue = create_queue(arr_len);

    // Add processes to arrival queue
    arrival_queue = create_queue(arr_len);
    for (int i = 0; i < arr_len; i++) {
        enqueue(arrival_queue, process_info[i].pid);
    }
}

int main(int argc, char *argv[]) {

    // Snapshot interval
    int snap_interval = atoi(argv[4]);
    const char *file_name = argv[2];

    // Use to select each scheduling algorithm
    int mode_select;
    // Main simulation loop
    for (mode_select = 0; mode_select < 5; mode_select++) {
        // Reset arrival queue
        init_queues(file_name);
        // First come, first served
        if (mode_select == 0) {
            // Initialize a new simulation
            init_sim(0);
            fcfs(snap_interval);
        }
    }

    return 0;
}
