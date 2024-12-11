#include <stdio.h>

int main(int argc, char *argv[], char *env[])
{
    extern char **environ;
    int i=0;
    for(;environ[i];i++)
    {
        printf("[%d]->%s\n",i,environ[i]);
    }
    return 0;
}
