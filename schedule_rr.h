#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list
void add_rr(char *name, int burst);

// invoke the scheduler
void schedule_rr();
