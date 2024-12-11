#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int g_val = 100;

int main() {
    pid_t id = fork();  // 创建一个新的进程

    if (id == 0) {  // 子进程
        g_val = 200;  // 子进程修改全局变量
        printf("child: PID: %d, PPID: %d, g_val: %d, &g_val: %p\n", getpid(), getppid(), g_val, &g_val);
    }
    else if (id > 0) {  // 父进程
        sleep(3);  // 父进程暂停3秒，等待子进程输出
        printf("father: PID: %d, PPID: %d, g_val: %d, &g_val: %p\n", getpid(), getppid(), g_val, &g_val);
    }
    else {  // fork失败
        perror("fork failed");
        exit(1);
    }
    return 0;
}
