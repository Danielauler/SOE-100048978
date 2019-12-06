#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SAIDA 1

void sqwv(int pin, int degree, int N)
{
    int t1 = (100 * degree + 4) / 9 + 1500;
    int t2 = 20000 - t1;
    int i;
    for (i = 0; i < N; i++)
    {
        digitalWrite(pin, HIGH);
        usleep(t1);
        digitalWrite(pin, LOW);
        usleep(t2);
    }
}

int main(void)
{
    int N = 40;
    wiringPiSetup();
    pinMode(SAIDA, OUTPUT);

    delay(1000);
    sqwv(SAIDA, -90, N);
    delay(2000);
    sqwv(SAIDA, 0, N);
    return 0;
}