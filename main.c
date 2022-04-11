// TODO: Update doc strings

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

int snapshot, arr_len;

void init_queues(const char *file_name) {

    // Read file content and store into arrival queue
    FILE *fp;
    arr_len = 0;
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
            // End of single process data
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

    // Add processes to arrival queue in order (file should be sorted)
    arrival_queue = create_queue(arr_len);
    for (int i = 0; i < arr_len; i++) {
        enqueue(arrival_queue, process_info[i].pid);
    }
}

int main(int argc, char *argv[]) {

    // Snapshot interval
    snapshot = atoi(argv[4]);
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
            fcfs();
        }
        // Shortest job first
        if (mode_select == 1) {
            init_sim(1);
            sjf();
        }
    }

    return 0;
}
