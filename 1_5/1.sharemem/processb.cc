// b要做的是先想办法获取shmid
// 然后将共享内存段连接到进程地址空间
// 然后通信
// 最后去关联  他不用做关闭共享内存的工作
// 关闭由a来完成

#include "comm.hpp"

extern Log log;

int main()
{
    // 1. 获取shmid
    int shmid = GetShm();
    //sleep(5);

    // 2. 使用函数shmat关联
    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    //sleep(5);
    int fd = open(FIFO_FILE, O_WRONLY); // 等待写入方打开之后，自己才会打开文件，向后执行， open 阻塞了！
    if(fd < 0)
    {
        log(Fatal, "error string: %s, error code: %d", strerror(errno), errno);
        exit(FIFO_OPEN_ERR);
    }

    // 一旦有了共享内存，挂接到自己的地址空间中，你直接把他当成你的内存空间来用即可！
    // 不需要调用系统调用
    // 3. ipc code
    while(true)
    {
        cout << "Please Enter@ ";
        fgets(shmaddr, 4096, stdin);

        write(fd,"c",1);
    }
    // 4. 使用shmdt去关联
    shmdt(shmaddr);
    //sleep(5);

    close(fd);
    return 0;
}