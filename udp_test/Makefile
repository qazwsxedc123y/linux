.PHONY:all
all:udpserver udpclient

udpserver:main.cc
	g++ -o $@ $^ -std=c++11
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread


.PHONY:clean
clean:
	rm -f udpserver udpclient