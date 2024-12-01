#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>

int main()
{
    printf("I am running.......\n");
    pid_t id = fork();
    if(id==-1)// 创建失败
    {
        printf("fork failed\n");
        return 1;
    }
    else if(id==0)// child
    {
        while(1)
        {
            printf("I am child process,PID:%d,PPID:%d\n",getpid(),getppid());
            sleep(1);
        }
    }
    else // parent
    {
        while(1)
        {
            printf("I am parent process,PID:%d,PPID:%d\n",getpid(),getppid());
            sleep(1);
        }
    }
    return 0;
}
