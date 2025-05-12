#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// 匿名管道
// 父 -> 写
// 子 -> 读

int main()
{
    int fd[2]; // 0 -> 读  1 -> 写
    int n = pipe(fd);
    if(n < 0)
    {
        perror("pipe");
        return 1;
    }
    pid_t id = fork();
    if(id == -1)
    {
        perror("fork");
        return 2;
    }
    else if(id == 0)
    {
        // child
        close(fd[1]); // 关闭写端
        char buff[64];
        while(1)
        {
            ssize_t s = read(fd[0], buff, sizeof(buff));
            if(s > 0)
            {
                buff[s] = '\0';
                printf("child read : %s\n", buff);
            }
            else if (s == 0)
            {
			    printf("read file end\n");
			    break;
		    }
		    else
            {
		    	printf("read error\n");
			    break;
            }
        }
        close(fd[0]);
        exit(0);
    }
    else
    {
        // father
        close(fd[0]); // 关闭读端
        const char* msg = "“i am father";
        while(1)
        {
            write(fd[1], msg, strlen(msg));
            sleep(1);
        }
        close(fd[1]);
        waitpid(id, NULL, 0);
        exit(0);
    }
    return 0;
}