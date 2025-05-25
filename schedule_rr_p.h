#ifndef SCHEDULE_RR_P_H
#define SCHEDULE_RR_P_H

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

void add_rr_p(char *name, int priority, int burst);
void schedule_rr_p();
void init_priority_queues(); //teste


#endif
