#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork(); // 创建子进程

    if (pid < 0) 
    {
        // fork 失败
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    
    if (pid == 0) 
    {
        // 子进程逻辑
        int count = 10;
        while (count--) 
        {
            printf("Child Process: PID = %d, PPID = %d\n", getpid(), getppid());
            sleep(1);
        }
        printf("Child Process (PID = %d) exiting...\n", getpid());
        exit(EXIT_SUCCESS);
    } 
    
    // 父进程逻辑
    int status = 0;
    pid_t ret = wait(&status); // 等待子进程结束

    if (ret > 0)
    {
        printf("Parent Process: Successfully waited for Child (PID = %d)\n", ret);

        if (WIFEXITED(status)) 
        {
            // 子进程正常退出
            printf("Child exited with code: %d\n", WEXITSTATUS(status));
        } 
        else if (WIFSIGNALED(status)) 
        {
            // 子进程被信号终止
            printf("Child terminated by signal: %d\n", WTERMSIG(status));
        }
    } 
    else 
    {
        // 等待失败
        perror("wait failed");
    }

    printf("Parent Process (PID = %d) exiting...\n", getpid());
    return EXIT_SUCCESS;
}
