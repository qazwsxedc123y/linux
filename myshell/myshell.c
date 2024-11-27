#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "$"
#define DELIM " \t"
#define ARGC_SIZE 32
#define LINE_SIZE 1024
#define EXIT_CODE 44

int lastcode = 0;
int quit = 0;
char commandline[LINE_SIZE];
char *_argv[ARGC_SIZE];
char pwd[LINE_SIZE];
extern char **environ;

const char *_getusername()
{
    return getenv("USER");
}

const char *_gethostname()
{
    return getenv("HOSTNAME");
}

void _getpwd()
{
    getcwd(pwd,sizeof(pwd));
}

void interact(char *cline, int size)
{
    _getpwd();
    printf(LEFT"%s@%s %s"RIGHT""LABLE" ", _getusername(), _gethostname(), pwd);
    char *s=fgets(cline,size,stdin);
    assert(s);
    (void)s;
    cline[strlen(cline)-1]='\0';
}

// ls -a -l
int splitstring(char cline[], char *_argv[])
{
    int i=0;
    _argv[i++]=strtok(cline,DELIM);
    while(_argv[i++]=strtok(NULL,DELIM));
    return i-1;
}

void NormalExcute(char *__argv[])
{
    pid_t id=fork();
    if(id<0)
    {
        perror("fork failed");
        return;
    }
    else if(id==0)
    {
        // 子进程使用进程替换执行命令
        // execvp
        execvp(__argv[0],__argv);
        exit(EXIT_CODE);
    }
    else 
    {
        // 父进程进程等待，防止僵尸进程的危害
        int status=0;
        pid_t rid=waitpid(id, &status, 0);
        if(rid == id) 
        {
            lastcode = WEXITSTATUS(status);
        }
    }
}

int buildCommand(char *_argv[], int _argc)
{
    if(_argc==2 && strcmp(_argv[0],"cd")==0)
    {
        chdir(_argv[1]);
        _getpwd();
        sprintf(getenv("PWD"), "%s", pwd);
        return 1;
    }
    else
    {

    }
     // 特殊处理一下ls
    if(strcmp(_argv[0], "ls") == 0)
    {
        _argv[_argc++] = "--color";
        _argv[_argc] = NULL;
    }
    return 0;
}

int main()
{
    while(!quit)
    {
        // 1.
        // 2. 交互问题，获得命令行，如ls -a -l
        interact(commandline, sizeof(commandline));

        // commandline -> "ls -a -l -n\0" -> "ls" "-a" "-l" "-n"
        // 3. 子串分割的问题，解析命令行
        int argc=splitstring(commandline,_argv);
        if(argc==0) continue;

        //for(int i=0;i<argc;i++)
        //{
        //    printf("[%d]:%s\n",i,_argv[i]);
        //}
        
        // 4. 指令的判断
         
        // 内建命令
        int n=buildCommand(_argv, argc);
         
        // 普通命令
        if(!n) NormalExcute(_argv);
    }
    return 0;
}
