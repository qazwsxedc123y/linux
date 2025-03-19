#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_NAME "myfifo"

int main()
{
    umask(0); // 将文件默认掩码设置为0
    if(mkfifo(FILE_NAME, 0666) < 0) // 调用mkfifo创建命名管道
    {
        // 失败
        perror("mkfifo");
        return 1;
    }
    printf("mkfifo success\n");
    return 0;
}