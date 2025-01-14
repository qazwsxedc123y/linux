#include <stdio.h>

int main()
{
    while(1){
        printf("I am a process, I am waiting signal, pid : %d\n",getpid());
        sleep(1);
    }
    return 0;
}