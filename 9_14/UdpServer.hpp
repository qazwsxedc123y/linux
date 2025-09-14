#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

const int SIZE = 128;
uint16_t defaultport = 8080;
std::string defaultip = "0.0.0.0";
class UdpServer
{
public:
    UdpServer(const uint16_t &port = defaultport, const std::string &ip = defaultip)
        :_sockfd(0)
        ,_port(port)
        ,_ip(ip)
    {}
    bool Socket()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            // 创建套接字失败
            std::cerr << "socket error" << std::endl;
			return false;
        }

        std::cout << "socket create success, sockfd: " << _sockfd << std::endl;

        return true;
    }
    bool Bind()
    {
        //填充网络通信相关信息
        struct sockaddr_in local;
        memset(&local, '\0', sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = inet_addr(_ip.c_str());

        //绑定
		if (bind(_sockfd, (struct sockaddr*)&local, sizeof(sockaddr)) < 0){ //绑定失败
			std::cerr << "bind error" << std::endl;
			return false;
		}

        std::cout << "bind success" << std::endl;

        return true;
    }
    bool InitServer()
    {
        // 检查socket创建是否成功
        if (!Socket()) 
        {
            return false;
        }
        
        // 检查绑定是否成功
        if (!Bind()) 
        {
            close(_sockfd);
            _sockfd = -1;
            return false;
        }
        return true;
    }
    void Start()
    {
        char buffer[SIZE];
        for(;;)
        {
            struct sockaddr_in peer;
			socklen_t len = sizeof(peer);
			ssize_t size = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer, &len);
            if (size > 0)
            {
                buffer[size] = '\0';
                int port = ntohs(peer.sin_port);
				std::string ip = inet_ntoa(peer.sin_addr);
				std::cout << ip << ":" << port << "# " << buffer << std::endl;
            }
            else
            {
                std::cerr << "recvfrom error" << std::endl;
            }

            std::string echo_msg = "server get->";
		    echo_msg += buffer;
		    sendto(_sockfd, echo_msg.c_str(), echo_msg.size(), 0, (struct sockaddr*)&peer, len);
        }
    }
    ~UdpServer()
    {
        if(_sockfd >= 0)
        {
            close(_sockfd);
        }
    }
private:
    int _sockfd; // 文件描述符
    int _port; //端口号
	std::string _ip; //IP地址
};