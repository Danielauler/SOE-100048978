#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i, pid_filho;
    for (i = 0; i < 3; i++)
    {
        pid_filho = fork();
        if (pid_filho == 0)
        {
            printf("Filho %d, pid %d\n", i + 1, getpid());
            return -1;
        }
        else
        {
            printf("Pai: %d\n", getpid());
            wait(NULL);
        }
    }
}