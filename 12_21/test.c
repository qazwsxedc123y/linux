#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Before : PTD is %d\n",getpid());
    pid_t id = fork();
    if(id==-1)
    {
        printf("fork error\n");
        return 1;
    }
    printf("After : PID is %d,return is %d\n",getpid(),id);
    return 0;
}
