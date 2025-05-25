#ifndef SCHEDULE_PA_H
#define SCHEDULE_PA_H

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

//Define o limiar de envelhecimento - quando uma tarefa atinge esse tempo de espera, sua prioridade aumenta
#define AGING_THRESHOLD 5

// Define o quantum: tempo máximo de execução de uma tarefa por vez
#define QUANTUM 10


void add_pa(char *name, int priority, int burst);
void schedule_pa();
void init_priority_queues();

#endif
