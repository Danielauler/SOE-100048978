#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

volatile int cnt=0;
static pthread_mutex_t mutexLock;

void *thread_function(void *arg) {
    int i;
    pthread_mutex_lock(&mutexLock);
    i = cnt;
    pthread_mutex_unlock(&mutexLock);
    while(i!=10) {
        pthread_mutex_lock(&mutexLock);
        if (i!=cnt) {
            printf("cnt = %d\n", cnt);
            i = cnt;
        }
        pthread_mutex_unlock(&mutexLock);
    }
    pthread_exit(0);
}