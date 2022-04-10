#ifndef PROJECT_3_SCHED_SIM_H
#define PROJECT_3_SCHED_SIM_H

struct CPUInfo {
    int time, snapshot;
    char state;
};

struct ProcessInfo {
    int pid, burst, priority, arrival, wait, turn_time, finished;
};

struct MethodStats {
    float avg_wt, avg_tt;
    int context_switches;
};

void init_sim(int method_num);

void fcfs(int snapshot);

#endif
