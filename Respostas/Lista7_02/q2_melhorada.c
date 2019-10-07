#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define N 100

int main()
{
    int pid;
    char buffer_saida[N];
    char buffer_entrada[N];
    int p_ppf[2], p_fpp[2];
    pipe(p_ppf);
    pipe(p_fpp);
    pid = fork();

    if (pid == 0)
    {
        strcpy(buffer_saida, "Pai, qual é a verdadeira essência da sabedoria?");
        write(p_fpp[1], buffer_saida, N);
        read(p_ppf[0], buffer_entrada, N);
        printf("PAI: %s\n", buffer_entrada);
        strcpy(buffer_saida, "Mas até uma criança de três anos sabe disso!");
        write(p_fpp[1], buffer_saida, N);
        read(p_ppf[0], buffer_entrada, N);
        printf("PAI: %s\n", buffer_entrada);

    }
    else
    {
        read(p_ppf[0], buffer_entrada, N);
        printf("FILHO: %s\n", buffer_entrada);
        strcpy(buffer_saida, "Não façais nada violento, praticai somente aquilo que é justo e equilibrado.");
        write(p_fpp[1], buffer_saida, N);
        read(p_ppf[0], buffer_entrada, N);
        printf("FILHO: %s\n", buffer_entrada);
        strcpy(buffer_saida, "Sim, mas é uma coisa difícil de ser praticada até mesmo por um velho como eu...");
        write(p_fpp[1], buffer_saida, N);
        wait(NULL);
    }
}