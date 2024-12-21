#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void show()
{
    printf("hello world");
    _exit(0);
}
int main()
{
    show();
    return 0;
}
