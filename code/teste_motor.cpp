#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <unistd.h>
#include <wiringPi.h>

#define SERVO 26

using namespace std;

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

void feederFunction(int delayTime, int N)
{
    sqwv(SERVO, 90, N);
    sleep(delayTime);
    sqwv(SERVO, 0, N);
};

int main()
{
    int N = 40;
    wiringPiSetup();
    pinMode(SERVO, OUTPUT);
    sqwv(SERVO, 0, N);
    thread feeder(feederFunction, 2, 40);
    feeder.join();
    return 0;
}