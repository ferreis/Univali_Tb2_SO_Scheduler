#include <stdio.h>
#include <stdlib.h>

#include "schedule_pa.h"
#include "list.h"
#include "task.h"
#include "CPU.h"

// Declaração das múltiplas filas de prioridade, uma para cada nível
struct node *priority_queues[MAX_PRIORITY + 1];

// Inicializa todas as filas de prioridade com NULL
void init_priority_queues() {
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        priority_queues[i] = NULL;
    }
}

// Adiciona uma tarefa na fila correspondente à sua prioridade
void add_pa(char *name, int priority, int burst) {
    // Verifica se a prioridade é válida
    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        printf("Erro: prioridade inválida %d\n", priority);
        exit(1);
    }

    // Aloca memória para a nova tarefa
    Task *newTask = (Task *)malloc(sizeof(Task));
    if (newTask == NULL) {
        perror("Erro ao alocar memória para a tarefa");
        exit(1);
    }

    // Preenche os campos da tarefa
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->waiting_time = 0;

    // Insere a tarefa na fila correspondente
    insert(&priority_queues[priority], newTask);
}

// Aplica o mecanismo de envelhecimento (aging) nas tarefas
void apply_aging() {
    // Percorre as filas da menor prioridade (maior número) para a maior
    for (int prio = MAX_PRIORITY; prio >= MIN_PRIORITY + 1; prio--) {
        struct node *current = priority_queues[prio];

        // Percorre a fila atual
        while (current != NULL) {
            Task *t = current->task;

            if (t->burst > 0) {
                // Incrementa o tempo de espera
                t->waiting_time += 1;

                // Se atingiu o limiar de envelhecimento
                if (t->waiting_time >= AGING_THRESHOLD) {
                    // Remove a tarefa da fila atual
                    struct node *to_move = current;
                    current = current->next;

                    delete(&priority_queues[prio], to_move->task);

                    // Aumenta a prioridade (diminui o valor)
                    t->priority -= 1;
                    t->waiting_time = 0;

                    // Insere a tarefa na fila de maior prioridade
                    insert(&priority_queues[t->priority], t);

                    continue;  // Continua para a próxima tarefa
                }
            }

            current = current->next;
        }
    }
}

// Função principal do escalonador Prioridade com Aging
void schedule_pa() {
    int tasksRemaining = 1;

    // Enquanto houver tarefas com burst > 0
    while (tasksRemaining) {
        tasksRemaining = 0;

        // Percorre todas as filas, da maior para a menor prioridade
        for (int prio = MIN_PRIORITY; prio <= MAX_PRIORITY; prio++) {
            struct node *current = priority_queues[prio];

            // Percorre a fila atual
            while (current != NULL) {
                Task *t = current->task;

                if (t->burst > 0) {
                    tasksRemaining = 1;
                    t->waiting_time = 0;  // Zera o tempo de espera ao executar

                    // Define o slice como o menor entre o burst restante e o quantum
                    int slice = (t->burst < QUANTUM) ? t->burst : QUANTUM;

                    // Executa a tarefa pelo slice
                    run(t, slice);
                    t->burst -= slice;

                    // Aplica o envelhecimento nas demais tarefas
                    apply_aging();

                    break;  // Executa apenas uma tarefa por ciclo
                }

                current = current->next;
            }

            if (tasksRemaining) {
                break;  // Sai do loop de prioridades após executar uma tarefa
            }
        }
    }
}
