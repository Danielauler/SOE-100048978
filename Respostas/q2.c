#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char **argv)
{
    int i;
    char *ls_args[] = {NULL, NULL};
    for (i = 1; i < argc; i++)
    {
        if (fork() == 0)
        {
            printf("%s:\n", argv[i]);
            ls_args[0] = argv[i];
            execvp(ls_args[0], ls_args);
            printf("Houve um erro na chamada %s\n", argv[i]);
            exit(-1);
        }
        else
        {
            wait(NULL);
        }
    }
    return 0;
}
