#ifndef SCHEDULE_EDF_H
#define SCHEDULE_EDF_H

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// Declaração das funções do EDF
void add_edf(char *name, int priority, int burst, int deadline);
void schedule_edf();

// Declaração da flag de tempo e controle do timer
extern volatile int time_expired;
extern volatile int timer_running;
extern int time_global;

#endif
