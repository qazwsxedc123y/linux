#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	printf("I am running...\n");
	pid_t id = fork();
	if(id == 0)
    { //child
		int count = 5;
		while(1)
        {
			printf("I am child...PID:%d, PPID:%d\n", getpid(), getppid(), count);
			sleep(1);
		}
	}
	else if(id > 0)
    { //father
		int count = 5;
		while(count){
			printf("I am father...PID:%d, PPID:%d, count:%d\n", getpid(), getppid(), count);
			sleep(1);
			count--;
		}
		printf("father quit...\n");
		exit(0);
	}
	else
    { //fork error
        perror("fork");
        return 1;
	}
	return 0;
} 
