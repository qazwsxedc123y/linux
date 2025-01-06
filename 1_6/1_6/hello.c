#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    umask(0);// 设置umask为0，这样在此进程下创建的文件所有权限都是默认全给的
    int fd = open("myfile", O_RDONLY);
    if(fd < 0){
        perror("open");
        return 1;
    }
    
    char buf[1024];
    const char *msg="hello world\n";
    int len=strlen(msg);

    while(1){
        ssize_t s = read(fd, buf, strlen(msg));//类比write
        if(s > 0){
            printf("%s",buf);
        }
        else{
            break;
        }
    }
    close(fd);
    return 0;
}