
#ifndef PROJECT_3_SCHED_FUNCTIONS_H
#define PROJECT_3_SCHED_FUNCTIONS_H

void print_cpu(float pid, float second_pid, float burst, float second_burst);

void handle_cpu_print();

void print_summary(int method_num);

void handle_finished_process();

void calculate_wait();

int handle_cycle(int method_num);

void handle_arrival_queue(int will_sort, char sort_by);

#endif
