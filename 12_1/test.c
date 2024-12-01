#include <stdio.h>
#include <unistd.h>

int main()
{
        printf("I am a process ...... PID: %d,PPID: %d\n",getpid(),getppid());
    return 0;
}
