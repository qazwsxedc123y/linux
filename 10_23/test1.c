#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
    int pid=fork();
    if(pid<0)
    {
        perror("fork");
        return 1;
    }
    else if(pid==0)
    {
        // 子进程
        while(1)
        {
            printf("This is the child process,this pid is:%d\n",getpid());
            sleep(1);
        }
    }
    else 
    {
        // 父进程
        printf("This is the parent process,this pid is:%d\n",getpid());
        sleep(1);
        exit(EXIT_SUCCESS);
        
    }
    return 0;
}

