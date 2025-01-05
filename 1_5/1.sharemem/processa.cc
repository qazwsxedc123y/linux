// a要做的就是先创建共享内存
// 然后在连接到进程地址空间
// 然后在使用，其是作为输入端（但共享内存通信可以是双向的，也可以是单向的，这取决于进程之间如何设计和使用共享内存。)这里是单向通信
// 最后再去关联，最后关闭共享内存


#include "comm.hpp"

extern Log log;

// 扩展：对共享内存添加管道，使得共享内存由同步的效果
// 记得要最后关闭close文件
// a为管道的读端
// b为管道的写端

int main()
{
    Init init;// 创建管道

    // 1. 先获取key，但是获取key要先调用函数ftok函数 // 这一步在comm.hpp内完成
    // 2. 然后调用函数shmget获取一个共享内存，这个共享内存是存在与物理空间上的
    int shmid = CreateShm();
    //sleep(5);

    // 3. 然后调用函数shmat连接到进程地址空间
    char *shmaddr = (char*)shmat(shmid, nullptr, 0);
    //sleep(5);

    int fd = open(FIFO_FILE, O_RDONLY);// 等待写入方打开之后，自己才会打开文件，向后执行， open 阻塞了！
    if(fd < 0)
    {
        log(Fatal, "error string: %s, error code: %d", strerror(errno), errno);
        exit(FIFO_OPEN_ERR);
    }

    // 4. ipc code
    // 一旦有人把数据写入到共享内存，其实我们立马能看到了！！
    // 不需要经过系统调用，直接就能看到数据了！

    struct shmid_ds shmds;
    while(true)
    {
        char c;
        ssize_t s=read(fd,&c,1);
        if(s==0) break;
        else if(s < 0) break;

        cout<<"client say@ "<<shmaddr<<endl; //直接访问共享内存
        sleep(1);

        shmctl(shmid, IPC_STAT, &shmds);
        cout << "shm size: " << shmds.shm_segsz << endl;
        cout << "shm nattch: " << shmds.shm_nattch << endl;
        printf("shm key: 0x%x\n",  shmds.shm_perm.__key);
        cout << "shm mode: " << shmds.shm_perm.mode << endl;        
    }

    // 5. 调用函数shmdt去关联，调用shmctl关闭共享内存
    shmdt(shmaddr);
    //sleep(5);
    shmctl(shmid, IPC_RMID, nullptr);
    
    close(fd);
    return 0;
}