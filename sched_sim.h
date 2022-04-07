#ifndef PROJECT_3_SCHED_SIM_H
#define PROJECT_3_SCHED_SIM_H

struct CPUInfo {
    int burst, time, active_process;
    char state[50];
    // TODO: Past run processes (ordered list)
};

struct ProcessInfo {
    int pid, burst, priority, arrival, wait, turn_time;
    // TODO: Other helper data?
};

struct MethodStats {
    float avg_wt, avg_tt;
    int context_switches;
};

#endif //PROJECT_3_SCHED_SIM_H
