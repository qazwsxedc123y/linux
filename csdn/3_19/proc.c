#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>  // POSIX 消息队列头文件
#include <fcntl.h>   // 定义 O_* 常量
#include <sys/stat.h> // 定义 mode 常量

#define QUEUE_NAME "/my_message_queue"  // 消息队列名称
#define MAX_MSG_SIZE 1024               // 消息的最大大小
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10) // 缓冲区大小

int main() {
    mqd_t mq;  // 消息队列描述符
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    int msg_priority = 0;

    // 设置消息队列属性
    attr.mq_flags = 0;          // 阻塞标志
    attr.mq_maxmsg = 10;        // 队列中最大消息数
    attr.mq_msgsize = MAX_MSG_SIZE; // 每条消息的最大大小
    attr.mq_curmsgs = 0;        // 当前队列中的消息数

    // 创建消息队列
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    printf("Message queue created: %s\n", QUEUE_NAME);

    // 发送消息到队列
    const char *msg_to_send = "Hello, Message Queue!";
    if (mq_send(mq, msg_to_send, strlen(msg_to_send) + 1, 0) == -1) {
        perror("mq_send");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }
    printf("Message sent: %s\n", msg_to_send);

    // 从队列接收消息
    ssize_t bytes_read = mq_receive(mq, buffer, MSG_BUFFER_SIZE, &msg_priority);
    if (bytes_read == -1) {
        perror("mq_receive");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';  // 确保字符串以 null 结尾
    printf("Message received: %s\n", buffer);

    // 关闭消息队列
    mq_close(mq);

    // 删除消息队列
    mq_unlink(QUEUE_NAME);
    printf("Message queue deleted: %s\n", QUEUE_NAME);

    return 0;
}