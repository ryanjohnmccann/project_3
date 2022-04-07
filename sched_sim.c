
#include <stdlib.h>
#include <stdio.h>

#include "sched_sim.h"
#include "queue.h"

struct CPUInfo cpu_info;
struct MethodStats method_stats[4];

extern arrival_queue;
extern ready_queue;
extern finished_queue;
extern process_info;

void init_sim(int method_num) {
    cpu_info.time = 0;
    cpu_info.burst = -1;
    cpu_info.active_process = -1;
    cpu_info.state = 'N';

    method_stats[method_num].avg_wt = 0;
    method_stats[method_num].avg_tt = 0;
    method_stats[method_num].context_switches = 0;
}

void fcfs(int snapshot) {
    printf("***** FCFS Scheduling *****\n");
    while (1) {

        break;
    }
}
