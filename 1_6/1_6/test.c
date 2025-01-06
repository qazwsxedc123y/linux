#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *pf = fopen("test.txt", "r");
    if(!pf){
        printf("fopen fail\n");
    }
    char buf[1024];
    const char* msg = "hello world\n";
    while(1){
        //注意返回值和参数，此处有坑，仔细查看man手册关于该函数的说明
        ssize_t s = fread(buf, 1, strlen(msg), pf);
        if(s > 0){
            buf[s] = 0;
            printf("%s",buf);
        }
        if(feof(pf)){
            break;
        }
    }
    fclose(pf);

    return 0;
}