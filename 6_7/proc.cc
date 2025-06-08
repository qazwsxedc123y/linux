#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

using namespace std;


void* rout(void *arg)
{
    char* msg = (char*)arg;
    for(int i = 0; i < 5; i++){
        printf("I am %s, pid: %d, ppid: %d, ID: %lu\n", msg, getpid(), getppid(), pthread_self());
        sleep(1);
        pthread_cancel(pthread_self());
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
    
    printf("I am main thread...pid: %d, ppid: %d, ID: %lu\n", getpid(), getppid(), pthread_self());

    for (int i = 0; i < 5; i++){;
		void* ret = NULL;
		pthread_join(tid[i], &ret);
		printf("thread %d[%lu]...quit, exitcode: %d\n", i, tid[i], (long)ret);
	}
    return 0;
}