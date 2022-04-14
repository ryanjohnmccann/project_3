
#ifndef PROJECT_3_SCHED_FUNCTIONS_H
#define PROJECT_3_SCHED_FUNCTIONS_H

void write_cpu_cycle(float pid, float second_pid, float burst, float second_burst, const char *file_name);

void handle_write_cpu_cycle(const char *file_name);

void write_summary(int method_num, const char *file_name);

void handle_finished_process();

void calculate_wait();

int handle_cycle(int method_num, const char *file_name);

void handle_arrival_queue(int will_sort, char sort_by);

#endif
