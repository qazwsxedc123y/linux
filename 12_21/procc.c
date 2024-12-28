#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    int status;

    // 创建子进程
    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0)
    {
        // child
        printf("i am child process , PID: %d\n", getpid());
        // 等待五秒
        sleep(5); 
        exit(0);
    } 
    else 
    {
        // parent
        printf("I am parent process, waiting for child (PID: %d) to exit...\n", pid);
        
        pid_t waited_pid = waitpid(pid, &status, 0);
        
        if (waited_pid == -1) 
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }

        // coredump分析子进程状态
        if (WIFEXITED(status)) 
        {
            printf("Child process exited WIFEXITED(status)\n");
        } 
        else if (WIFSIGNALED(status)) 
        {
            printf("Child process WIFSIGNALED(status)\n");
        }
        else if (WIFSTOPPED(status)) 
        {
            printf("Child process WIFSTOPPED(status)\n");
        }

        printf("Parent process exiting...\n");
    }

    return 0;
}
