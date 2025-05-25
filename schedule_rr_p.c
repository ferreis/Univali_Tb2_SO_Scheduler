#include <stdio.h>
#include <stdlib.h>
#include "schedule_rr_p.h"
#include "list.h"
#include "task.h"
#include "CPU.h"

// Declaração das múltiplas filas de prioridade, uma para cada nível
struct node *priority_queues[MAX_PRIORITY + 1];

// Função para inicializar todas as filas de prioridade
// Evita erros como "Falha de segmentação" ao acessar posições não inicializadas
void init_priority_queues() {
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        priority_queues[i] = NULL;
    }
}

// Adiciona uma nova tarefa na fila de prioridade correspondente
void add_rr_p(char *name, int priority, int burst) {
    // Valida se a prioridade está dentro dos limites permitidos
    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        printf("Erro: prioridade inválida %d\n", priority);
        exit(1);
    }

    // Aloca memória para a nova tarefa
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    // Insere a nova tarefa na fila da prioridade correspondente
    insert(&priority_queues[priority], newTask);
}

// Função principal do escalonador Round Robin com Prioridade
void schedule_rr_p() {
    int tasksRemaining = 1;

    // Continua enquanto houver tarefas com burst > 0
    while (tasksRemaining) {
        tasksRemaining = 0;

        // Percorre todas as filas de prioridade, da maior (menor valor) para a menor
        for (int prio = MIN_PRIORITY; prio <= MAX_PRIORITY; prio++) {
            struct node *current = priority_queues[prio];

            // Percorre a fila de tarefas da prioridade atual
            while (current != NULL) {
                Task *t = current->task;

                if (t->burst > 0) {
                    tasksRemaining = 1;

                    // Define o tempo de execução como o menor entre o burst e o quantum
                    int slice = (t->burst < QUANTUM) ? t->burst : QUANTUM;

                    // Executa a tarefa pelo slice determinado
                    run(t, slice);

                    // Atualiza o burst restante da tarefa
                    t->burst -= slice;
                }

                current = current->next;
            }
        }
    }
}
