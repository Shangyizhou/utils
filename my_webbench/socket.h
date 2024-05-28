#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <arpa/inet.h>

class Client {
public:
    Client(const std::string& host, const uint32_t& port);
    ~Client();
    void BindAddress();
    int Connect();
private:
    int _connfd;
    std::string _host;
    uint32_t _port;
    sockaddr_in _addr;
};

Client::Client(const std::string& host, const uint32_t& port) 
    : _host(host), _port(port) {}

void Client::BindAddress() {
    unsigned long inaddr;
    struct hostent *hp;
    // 将点分十进制IP地址转换为网络字节序的32位二进制数
    inaddr = inet_addr(_host.c_str()); 
    if (inaddr != INADDR_NONE) {
         memcpy(&_addr.sin_addr, &inaddr, sizeof(inaddr));
    } else {
        /**
         * gethostbyname 通过我们传入一个域名返回一个 struct hostent 结构体
         * - h_name：主机的官方名称。
         * - h_aliases：指向一个包含备用主机名的数组的指针。
         * - h_addrtype：主机的地址类型。对于IPv4地址，应设置为AF_INET。
         * - h_length：地址的长度，以字节为单位。
         * - h_addr_list：指向一个包含主机的网络地址的数组的指针。
        */
        hp = gethostbyname(_host.c_str());
        if (hp == nullptr) {
            std::cout << "gethostbyname failed" << std::endl;
            return;
        }     
        memcpy(&_addr.sin_addr, hp->h_addr_list[0], hp->h_length);       
    }
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
}


int Client::Connect() {
    _connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_connfd < 0) {
        std::cout << "Error socket" << std::endl;
        return -1;
    }
    
    if (connect(_connfd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
        std::cout << "Error connecting" << std::endl;
        return -1;
    }
    return _connfd;
}