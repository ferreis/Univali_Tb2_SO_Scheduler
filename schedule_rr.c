#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedule_rr.h"
#include "list.h"
#include "task.h"
#include "CPU.h"

// Lista global de tarefas utilizada pelo escalonador RR
struct node *task_list_rr = NULL;

// Função para adicionar uma tarefa à lista de tarefas
void add_rr(char *name, int burst) {
    // Aloca espaço para nova tarefa
    Task *newTask = (Task *)malloc(sizeof(Task));
    if (newTask == NULL) {
        printf("Erro: Falha ao alocar memória para nova tarefa.\n");
        exit(1);
    }

    // Preenche os campos da tarefa
    newTask->name = name;
    newTask->burst = burst;

    // Insere a nova tarefa na lista encadeada
    insert(&task_list_rr, newTask);
}

// Função para realizar o escalonamento e execução das tarefas
// utilizando o algoritmo Round-Robin
void schedule_rr() {
    struct node *current;
    int tasksRemaining = 1;

    // Loop principal: continua enquanto houver tarefas com burst > 0
    while (tasksRemaining) {
        tasksRemaining = 0;
        current = task_list_rr;

        // Percorre a lista de tarefas
        while (current != NULL) {
            Task *t = current->task;

            // Verifica se ainda há burst para executar
            if (t->burst > 0) {
                tasksRemaining = 1;

                // Define o slice como o menor entre o burst restante e o quantum
                int slice = (t->burst < QUANTUM) ? t->burst : QUANTUM;

                // Executa a tarefa pelo slice definido
                run(t, slice);

                // Atualiza o burst restante
                t->burst -= slice;
            }

            // Avança para a próxima tarefa na lista
            current = current->next;
        }
    }
}
