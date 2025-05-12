#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
const int size = 4096; 
const char* pathname = "/home/haha";
const int proj_id = 0x6666;
key_t GetKey()
{
    key_t k = ftok(pathname, proj_id);//返回一个指向 C 风格字符串的指针（即以 '\0' 结尾的字符数组）
    return k;
}
int main() 
{
    key_t key = GetKey();
    int shmid = shmget(key, size, 0666);
    char* shm = shmat(shmid, NULL, 0);

    printf("Process B read: %s\n", shm);

    // 分离共享内存
    shmdt(shm);
    
    return 0;
}