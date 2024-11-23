#include <stdio.h>  
#define Linux 1  
#define MAX 100
int main()
{  

    #if Linux
        printf("hello Linux\n");  
    #else  
        printf("hello Windows\n");    
    #endif
        
    #ifdef Linux  
        printf("Debug mode is enabled.\n");  
    #endif // 这个指令标记了 #ifdef 的结束  

    printf("%d\n", MAX);  
    printf("Program is running...\n");  

    return 0;  
}  
