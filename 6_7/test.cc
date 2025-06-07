#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

using namespace std;

__thread  int g_val = 100;

void* rout(void *arg)
{
    char* msg = (char*)arg;
    int i = *(msg + 7) - '0';
    g_val *= i;
    for( ; ; ){
        printf("I am %s, pid: %d, ppid: %d, LWP: %d, g_val = %d\n", msg, getpid(), getppid(), gettid(), g_val);
        sleep(1);
    }
}

int main()
{
    pthread_t tid[5];
    int ret;
    for(int i = 0; i < 5; i++){
        char* buffer = (char*)malloc(64);
        sprintf(buffer, "thread %d", i);
        if(pthread_create(&tid[i], NULL, rout, (void *)buffer) != 0)
        {
            fprintf(stderr, "pthread_create : %s\n",strerror(ret));
            exit(1);
        }
    }

    int i;
    for( ; ; ){
        printf("I am main thread...pid: %d, ppid: %d, LWP: %d\n", getpid(), getppid(), gettid());
        sleep(1);
    }
    return 0;
}