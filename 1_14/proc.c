#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("log.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
    }
    else
    {
        int fd1 = fileno(fp);                 // 获取底层的文件描述符
        printf("File descriptor: %d\n", fd1); // 打印文件描述符

    }
    fclose(fp);
    return 0;
}
