#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



FILE *fp = fopen("log1.txt", "r");
if (fp == NULL) {
    perror("Error opening file");
} else {
    int fd = fileno(fp);  // 获取底层的文件描述符
    printf("File descriptor: %d\n", fd);  // 打印文件描述符
    fclose(fp);
}