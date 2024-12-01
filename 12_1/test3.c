#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// 使系统进入挂起（休眠）状态
void enter_deep_sleep() {
    // 使用 system() 调用系统命令进入挂起状态
    int ret = system("systemctl suspend"); // 系统挂起命令
    if (ret == -1) {
        printf("系统无法进入深度睡眠模式\n");
    } else {
        printf("系统进入深度睡眠模式\n");
    }
    while(1)
        sleep(1);
}

int main(void) {
    // 程序初始化
    printf("系统即将进入深度睡眠模式...\n");

    // 进入深度睡眠
    enter_deep_sleep();

    // 程序在此处暂停，直到系统从挂起状态唤醒
    printf("程序恢复执行。\n");

    return 0;
}
