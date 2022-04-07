// TODO: Update doc strings
// TODO: Cite where you took code

#include <stdio.h>
#include <stdlib.h>

#include "sched_sim.h"

// TODO: Find a better method for this
int get_arr_size(const char *file_name) {
    FILE *fp;
    ssize_t read;
    size_t len = 0;
    int arr_len = 0;
    char *line = NULL;

    fp = fopen(file_name, "r");

    if (NULL == fp) {
        printf("File cannot be opened \n");
    }
    // To determine the size of our array of structs by counting number of processes (lines) in the file
    while ((read = getline(&line, &len, fp)) != -1) {
        arr_len += 1;
    }

    fclose(fp);
    return arr_len;
}

int main(int argc, char *argv[]) {

    // Snapshot interval
    int snap_interval = atoi(argv[4]);
    const char *file_name = argv[2];

    FILE *fp;
    int curr_num = 0;
    int arr_index = 0;
    int pid = 0;
    // Determines when we've finished reading values for a single process
    int data_count = 1;

    struct ProcessInfo process_info[get_arr_size(file_name)];

    fp = fopen(file_name, "r");

    // Extract data into array of structs
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
        }
        data_count += 1;
    }

    fclose(fp);

    return 0;
}
