// TODO: Update doc strings

#include <stdio.h>
#include <stdlib.h>

#include "sched_sim.h"

void handle_process_data(char *data_file) {
    FILE *fp;
    ssize_t read;
    size_t len = 0;
    char *line = NULL;
    int arr_len = 0;

    fp = fopen(data_file, "r");

    if (NULL == fp) {
        printf("File cannot be opened \n");
    }
    // To determine the size of our array of structs
    while ((read = getline(&line, &len, fp)) != -1) {
        arr_len += 1;
    }
    struct ProcessInfo process_info[arr_len];

    // Extract data into array of structs
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {

    // Snapshot interval
    int snap_interval = atoi(argv[4]);

    handle_process_data(argv[2]);

    return 0;
}
