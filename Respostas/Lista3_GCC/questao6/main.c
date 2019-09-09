#include <stdio.h>

int main(int argc, char **argv)
{
    printf("Olá %s \n", argv[0]);
    printf("Número de entradas: %d", argc - 1);

	return 0;
}