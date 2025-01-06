#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd1 = open("test.txt", O_RDONLY | O_CREAT, 0666);
    printf("fd1:%d\n", fd1);
    FILE *fp = fopen("log1.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
    }
    else
    {
        int fd3 = fileno(fp);                 // 获取底层的文件描述符
        printf("File descriptor: %d\n", fd3); // 打印文件描述符

    }
    int fd2 = open("log2.txt", O_RDONLY | O_CREAT, 0666);
    printf("fd1:%d\n", fd2);
    fclose(fp);
    close(fd1);
    close(fd2);
    return 0;
}
