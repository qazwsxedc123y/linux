#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    // 如果文件不存在在创建文件  读写打开
    FILE* fd = fopen("bite.txt", "w+");
    if (fd < 0)
    {
		perror("fopen");
		return 1;
    }
    // “linux so easy!”
    const char* msg = "linux so easy!";
    fwrite(msg, 1, strlen(msg), fd);
    // 在从文件当中读出文件当中的内容， 打印到标准输出当中； 关闭文件描述符
    fseek(fd, 0, SEEK_SET); // 使用 fseek 将文件指针移动到开头
    char inbuff[1024] = {0};
    size_t s = fread(inbuff, 1, sizeof(inbuff) - 1, fd); 
    inbuff[s] = '\0';
    printf("%s\n",inbuff);
    fclose(fd);
    return 0;
}