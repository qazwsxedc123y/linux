#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("test.txt", O_RDONLY);
    // 文件操作代码...
    printf("%d\n",fd);
    close(fd);
    return 0;
}