#include <stdio.h>

int main(int argc, char **argv)
{
	int i;
    for(i=0; argc; i++) {
        if(argc>i + 1)
            printf("%s", argv[i]);
    }
	return 0;
}