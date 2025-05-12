#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

// 1.考察使用共享内存实现进程间通信， 理解共享内存原理， 考察shmget, shmat, shmdt， shmctl函数的使用， 理解共享内存的生命周期

// 2.要求：使用代码创建一个共享内存， 支持两个进程进行通信

//  进程A 向共享内存当中写 “i am process A”

//  进程B 从共享内存当中读出内容，并且打印到标准输出

// 3.提交要求：提交代码截图， 提交代码执行之后的截图
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
    int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
    char* shm = (char*)shmat(shmid, NULL, 0);

    strcpy(shm, "i am process A");
    printf("process A send: %s\n", shm);

    while(1);

    // 分离共享内存
    shmdt(shm);

    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}