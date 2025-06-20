#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void *thread_run(void *arg)
{
    pthread_detach(pthread_self());
    printf("%s\n",(char*)arg);
    return nullptr;
}

int main()
{
    pthread_t tid;
    if(pthread_create(&tid, NULL, thread_run, (void *)"thread1 run...") != 0){
        printf("create thread error\n");
        return 1;
    }

    // 同样也可以这样设置分离
    // pthread_detach(tid);
    
    int ret = 0;
    sleep(1);

    if(pthread_join(tid, NULL) == 0){
        printf("pthread wait success\n");
        ret = 0;
    }
    else{
        printf("pthread wait failed\n");
        ret = 1;
    }
    return ret;
}