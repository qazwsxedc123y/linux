#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t id=fork();
    if(id==-1)
    {
        perror("fork");// fork失败，会输出错误信息
        return 1;
    }
    else if(id>0)
    {
        printf("parent[%d] is sleeping...\n",getpid());
        sleep(30);
    }
    else 
    {
        printf("child[%d] is begin Z...\n",getpid());
        sleep(5);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
