#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "$"
#define DELIM " "
#define ARGC_SIZE 32
#define LINE_SIZE 1024
#define EXIT_CODE 44
 
int lastcode = 0;
char commandline[LINE_SIZE];
char *_argv[ARGC_SIZE];
char pwd[LINE_SIZE];
extern char **environ;


// [用户名 主机名 所在路径]$

const char *_getusername()
{
    return getenv("USER");
}

const char *_gethostname() {
    static char hostname[256];
    gethostname(hostname, sizeof(hostname)); 
    return hostname;
}
void _getpwd()
{
    char tmp[LINE_SIZE];
    getcwd(tmp,sizeof(tmp));
    // 查找最后一个/
    char *last_slash = strrchr(tmp, '/');
    // 拷贝到pwd中
    strcpy(pwd, last_slash + 1);
}

void interact(char *cline, int size)
{
    _getpwd();
    printf(LEFT"%s@%s %s"RIGHT""LABLE" ", _getusername(), _gethostname(), pwd);
    // 获取命令行。
    char *s=fgets(cline,size,stdin);
    assert(s);
    (void)s;
    cline[strlen(cline)-1]='\0';
}

int splitstring(char cline[], char *_argv[])
{
    // 解析命令行。
    // 比如 ls -a
    int i = 0;
    _argv[i++] = strtok(cline,DELIM);
    while(_argv[i++]=strtok(NULL,DELIM));
    return i-1;
}


void NormalExcute(char *__argv[])
{
    // 比如 ls -a
    // 创建子进程。
    pid_t id=fork();
    if(id < 0)
    {
        perror("fork failed");
        return ;
    }
    else if(id == 0)
    {
        // 替换子进程。
        // 子进程使用进程替换执行命令
        execvp(__argv[0],__argv);
        exit(EXIT_CODE);
    }
    else 
    {
        // 等待子进程退出。
        // 父进程进程等待，防止僵尸进程的危害
        int status=0;
        pid_t rid=waitpid(id, &status, 0);
        if(rid == id) 
        {
            lastcode = WEXITSTATUS(status);
        }
    }
}

int main()
{
    // 死循环
    while (1)
    {
        // 交互问题，获得命令行
        interact(commandline, sizeof(commandline));
        // 子串分割的问题，解析命令行
        int argc=splitstring(commandline,_argv);
        if(argc == 0) continue;
        for(int i=0;i<argc;i++)
        {
           printf("[%d]:%s\n",i,_argv[i]);
        }

        // 开始创建子进程进行进程替换
        NormalExcute(_argv);
    }
    return 0;
}