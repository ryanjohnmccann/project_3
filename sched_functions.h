
#ifndef PROJECT_3_SCHED_FUNCTIONS_H
#define PROJECT_3_SCHED_FUNCTIONS_H

void print_cpu(float pid, float second_pid, float burst);

void handle_cpu_print();

void print_summary(int method_num);

void handle_finished_process();

void calculate_wait();

#endif