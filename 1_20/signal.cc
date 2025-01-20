#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sigchld_handler(int signum) {
    int status;
    pid_t pid;
    // 回收所有已退出的子进程
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("Child %d exited with status: %d\n", pid, WEXITSTATUS(status));
        }
    }
}

int main() {
    signal(SIGCHLD, sigchld_handler); // 注册 SIGCHLD 信号处理函数

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            printf("Child %d running...\n", getpid());
            sleep(2); // 模拟工作
            return i; // 返回退出码
        }
    }

    // 父进程继续执行自己的逻辑
    while (1) {
        printf("Parent process doing other work...\n");
        sleep(1);
    }

    return 0;
}