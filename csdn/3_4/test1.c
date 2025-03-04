#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	int fd[2] = { 0 };
	if (pipe(fd) < 0) //使用pipe创建匿名管道
    { 
		perror("pipe");
		return 1;
	}
	pid_t id = fork(); //使用fork创建子进程
	if (id == 0)
    {
		//child
		close(fd[0]); //子进程关闭读端
		//子进程向管道写入数据
		const char* msg = "hello father, I am child...";
		int count = 10;
		while (count--)
        {
			write(fd[1], msg, strlen(msg));
			sleep(1);
		}
		close(fd[1]); //子进程写入完毕，关闭文件
		exit(0);
	}
	//father
	close(fd[1]); //父进程关闭写端
	close(fd[0]); //父进程直接关闭读端（导致子进程被操作系统杀掉）
	int status = 0;
	waitpid(id, &status, 0);
	printf("child get signal:%d\n", status & 0x7F); //打印子进程收到的信号  0x7f->1111111 低七位
	return 0;
}
