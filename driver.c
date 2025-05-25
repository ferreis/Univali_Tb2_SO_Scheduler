#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusão dos cabeçalhos de cada algoritmo e das estruturas de dados
#include "task.h"
#include "list.h"
#include "schedule_rr_p.h"
#include "schedule_edf.h"
#include "schedule_rr.h"
#include "schedule_pa.h"

#define SIZE 100  // Tamanho máximo para leitura de cada linha do arquivo

// Ative o algoritmo desejado descomentando uma das opções abaixo
//#define RR_P
//#define EDF
//#define RR
#define PA

int main(int argc, char *argv[])
{
FILE *in;           // Ponteiro para o arquivo de entrada
char *temp;         // Ponteiro temporário para manipular cada linha
char task[SIZE];    // Buffer para armazenar cada linha lida
char *name;         // Nome da tarefa

#ifdef RR
int burst;          // Tempo de execução, usado em RR
#endif

#ifdef RR_P
int priority;       // Prioridade da tarefa, usado em RR_P
int burst;         // Tempo de execução
#endif

#ifdef EDF
int priority;       // Prioridade, usado em EDF
int burst;          // Tempo de execução
int deadline;       // Deadline, usado apenas em EDF
#endif

#ifdef PA
int priority;       // Prioridade, usado em PA
int burst;          // Tempo de execução
#endif


    // Verifica se o usuário passou o nome do arquivo
    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo para leitura
    in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Loop de leitura de cada linha do arquivo
    while (fgets(task, SIZE, in) != NULL) {
        temp = strdup(task);  // Duplica a string lida para poder usar strsep

        name = strsep(&temp, ",");  // Extrai o nome da tarefa

        // Para cada algoritmo, extrai os parâmetros necessários e adiciona a tarefa

        #ifdef RR
            burst = atoi(strsep(&temp, ","));
            add_rr(name, burst);
        #endif

        #ifdef RR_P
            priority = atoi(strsep(&temp, ","));
            burst = atoi(strsep(&temp, ","));
            add_rr_p(name, priority, burst);
        #endif

        #ifdef EDF
            priority = atoi(strsep(&temp, ","));
            burst = atoi(strsep(&temp, ","));
            deadline = atoi(strsep(&temp, ","));
            add_edf(name, priority, burst, deadline);
        #endif

        #ifdef PA
            priority = atoi(strsep(&temp, ","));
            burst = atoi(strsep(&temp, ","));
            add_pa(name, priority, burst);
        #endif

        free(temp);  // Libera a memória alocada por strdup
    }

    fclose(in);  // Fecha o arquivo

    // Chama o escalonador correspondente conforme o algoritmo definido

    #ifdef RR
        schedule_rr();
    #endif

    #ifdef RR_P
        schedule_rr_p();
    #endif

    #ifdef EDF
        schedule_edf();
    #endif

    #ifdef PA
        schedule_pa();
    #endif

    return 0;  // Fim do programa
}
