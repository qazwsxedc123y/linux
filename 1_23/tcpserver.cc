#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netinet/tcp.h>

const int port = 8081;
const int backlog = 2;

void* Routine(void* arg)
{
	pthread_detach(pthread_self());
	int fd = *(int*)arg;
	delete (int*)arg;
	while (1){
		std::cout << "socket " << fd << " is serving the client" << std::endl;
		sleep(1);
	}

	return nullptr;
}

int main()
{
    // 创建监听套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0){
        std::cerr << "socket error" << std::endl;
        return 1;
    }
    
    // 设置SO_REUSEADDR，避免TIME_WAIT状态影响重启
    int opt = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt error" << std::endl;
    }
    
    // 绑定
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_port = htons(port);
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
        std::cerr << "bind error" << std::endl;
        close(listen_sock);
        return 2;
    }
    
    // 监听
    if (listen(listen_sock, backlog) < 0){
        std::cerr << "listen error" << std::endl;
        close(listen_sock);
        return 3;
    }
    
    std::cout << "Server started on port " << port << std::endl;
    
    // 启动服务器
   for (;;){
		//不调用accept获取连接
	}
    
    close(listen_sock);
    return 0;
}