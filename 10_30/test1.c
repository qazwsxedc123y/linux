#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>

int main()
{
    printf("I am running.......\n");
    fork();
    while(1)
    {
        printf("I am a process PID: %d,PPID: %d\n",getpid(),getppid());
        sleep(1);
    }
    return 0;
}
