#include <stdio.h>
#include <stdlib.h>

int g_unval;  // 修正变量名中的空格
int g_val = 100;

int main(int argc, char* argv[], char* envp[]) {
    int i;
    // 打印代码段地址
    printf("code addr: %p\n", main);  // main函数的地址

    // 打印只读常量区地址
    char* str = "hello world";  // 字符串字面量，存储在只读数据区
    printf("read only addr: %p\n", str);  // 字符串常量的地址

    // 打印已初始化数据段的地址
    printf("init addr: %p\n", &g_val);  // 已初始化全局变量g_val的地址

    // 打印未初始化数据段的地址
    printf("uninit addr: %p\n", &g_unval);  // 未初始化全局变量g_unval的地址

    // 打印堆区地址
    int* p = (int*)malloc(10 * sizeof(int));  // 动态分配10个int类型的内存
    printf("heap addr: %p\n", p);  // 堆区的地址

    // 打印栈区地址
    printf("stack addr: %p\n", &str);  // 栈上局部变量str的地址
    printf("stack addr: %p\n", &p);    // 栈上局部变量p的地址

    // 打印命令行参数的地址
    for (i = 0; i < argc; i++) {
        printf("args addr: %p\n", argv[i]);  // 打印每个命令行参数的地址
    }

    // 打印环境变量的地址
    i=0;
    while (envp[i]) {
        printf("env addr: %p\n", envp[i]);  // 打印每个环境变量的地址
        i++;
    }
    return 0;
}
