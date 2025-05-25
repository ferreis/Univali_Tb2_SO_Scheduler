#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "schedule_edf.h"
#include "list.h"
#include "task.h"
#include "CPU.h"

// Lista de tarefas EDF
struct node *task_list_edf = NULL;

// Flag para controle de tempo: indica quando o tempo do slice acabou
volatile int time_expired = 0;

// Controle para encerrar a thread do timer de forma segura
volatile int timer_running = 1;

// Variável para simular o tempo global do sistema
int time_global = 0;

// Protótipo da função do timer
void* timer_thread(void *arg);

// Função para adicionar uma tarefa ao escalonador EDF
void add_edf(char *name, int priority, int burst, int deadline) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    if (newTask == NULL) {
        perror("Erro ao alocar memória para a tarefa");
        exit(1);
    }
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->deadline = deadline;
    newTask->timestamp = -1; // Inicializa o timestamp como -1 para identificar se já foi executada

    insert(&task_list_edf, newTask);
}

// Função que busca a tarefa com o menor deadline na lista
struct node* find_earliest_deadline(struct node *list) {
    struct node *temp = list;
    struct node *earliest = list;

    while (temp != NULL) {
        if (temp->task->deadline < earliest->task->deadline) {
            earliest = temp;
        }
        temp = temp->next;
    }
    return earliest;
}

// Função da thread que simula um timer de hardware
void* timer_thread(void *arg) {
    while (timer_running) {
        sleep(1);            // Simula a passagem de uma unidade de tempo
        time_expired = 1;   // Indica que o tempo do slice terminou
    }
    return NULL;
}

// Função principal do escalonador EDF
void schedule_edf() {
    pthread_t timer_tid;

    // Cria a thread responsável por simular o timer
    if (pthread_create(&timer_tid, NULL, timer_thread, NULL) != 0) {
        perror("Erro ao criar a thread do timer");
        exit(1);
    }

    // Enquanto existirem tarefas na lista, o escalonador continua executando
    while (task_list_edf != NULL) {
        struct node *earliest = find_earliest_deadline(task_list_edf);
        Task *t = earliest->task;

        // Marca o tempo de início da tarefa
        if (t->timestamp == -1) {
            t->timestamp = time_global;
        }

        int finish_time = t->timestamp + t->burst;

        // Verifica se a tarefa conseguirá terminar antes do deadline
        if (finish_time > t->deadline) {
            printf("AVISO: A Tarefa [%s] perdeu seu prazo! (Prazo: %d, Conclusão: %d)\n",
                t->name, t->deadline, finish_time);
        } else {
            printf("A Tarefa [%s] será concluída antes do prazo. (Prazo: %d, Conclusão: %d)\n",
                t->name, t->deadline, finish_time);
        }

        // Executa a tarefa por unidade de tempo
        while (t->burst > 0) {
            while (!time_expired) {
                // Aguarda o estouro do tempo do slice
            }

            time_expired = 0;    // Reinicia a flag para o próximo slice
            run(t, 1);           // Executa a tarefa por 1 unidade de tempo
            t->burst -= 1;       // Reduz o burst restante
            time_global += 1;    // Incrementa o tempo global
        }

        // Após execução, remove a tarefa da lista e libera a memória
        delete(&task_list_edf, t);
        free(t);
    }

    // Finaliza a thread do timer de forma segura
    timer_running = 0;
    pthread_join(timer_tid, NULL);
}
