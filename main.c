// TODO: Write to output files
// TODO: Print final final summary results
// TODO: Remove context switches from that struct
// TODO: Handle various ties (SJF, STCF, Priority)

// Standard imports
#include <stdio.h>
#include <stdlib.h>

// Header file imports
#include "sched_sim.h"
#include "queue.h"

// Global variables
struct Queue *arrival_queue;
struct Queue *ready_queue;
struct Queue *sequence_queue;
struct ProcessInfo process_info[10000];

int snapshot, arr_len;

/***
 * Reads a file and adds them to the arrival queue
 * @param file_name The filename of the file to be read
 */
void init_arrival_queue(const char *file_name) {

    // Read file content and store into arrival queue
    FILE *fp;
    int arr_index = 0;
    int pid = 0;
    int data_count = 1;
    int curr_num;

    arr_len = 0;

    // Initialize and clean arrival queue
    arrival_queue = create_queue();
    clean_queue(arrival_queue);

    fp = fopen(file_name, "r");
    while (!feof(fp)) {
        fscanf(fp, "%d", &curr_num);
        // Initialize new process
        if (data_count == 1) {
            process_info[arr_index].pid = pid;
            // Add to the arrival queue
            enqueue(arrival_queue, pid);
            process_info[arr_index].burst = curr_num;
            process_info[arr_index].wait = 0;
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

}

void init_output_file(const char *file_name) {
    FILE *fp;

    fp = fopen(file_name, "w");
    fclose(fp);
}

// Controls the program
int main(int argc, char *argv[]) {

    // Snapshot interval
    snapshot = atoi(argv[3]);
    const char *input_file_name = argv[1];
    const char *output_file_name = argv[2];

    init_output_file(output_file_name);

    // Main simulation loop
    for (int mode_select = 0; mode_select < 5; mode_select++) {
        ready_queue = create_queue();
        sequence_queue = create_queue();
        // Reset arrival queue and get ready for another scheduling simulation
        init_arrival_queue(input_file_name);
        init_sim(mode_select);
        if (mode_select == 0) {
            fcfs(output_file_name);
        } else if (mode_select == 1) {
            sjf(output_file_name);
        } else if (mode_select == 2) {
            stcf(output_file_name);
        } else if (mode_select == 3) {
            round_robin(output_file_name);
        } else if (mode_select == 4) {
            priority(output_file_name);
        }
        clean_queue(ready_queue);
        clean_queue(sequence_queue);
    }

    write_final_summary(output_file_name);

    return 0;
}
