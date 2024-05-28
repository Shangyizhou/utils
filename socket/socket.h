#include <iostream>
#include <string>
#include <functional>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <arpa/inet.h>

class Socket {
public:
    Socket(const std::string& ip, const uint32_t& port);
    ~Socket();

    int GetDefaultSocket();

    // void StartLoop();

    void SetReadcallback(Socket::Readcallback callback) {
        m_read_callback = callback;
    }

private:
    void Bind();
    void Listen();
    void Accept();

    typedef std::function<void()> ReadCallback;

    int _listenfd;
    std::string _ip;
    uint32_t _port;
    Readcallback m_read_callback;
};

Socket::Socket(const std::string& ip, const uint32_t& port) : _ip(ip), _port(port) {
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenfd < 0) {
        std::cout << "socket error" << std::endl;
    }
}

int Socket::GetDefaultSocket() {
    Bind();
    Listen();
    return _listenfd;
}

void Socket::Bind() {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(_port);
    socklen_t addr_len = sizeof(addr);
    int res = bind(_listenfd, (sockaddr*)&addr, addr_len);
    if (res < 0) {
        std::cout << "bind failed" << std::endl;
    }
}

void Socket::Listen() {
    int res = listen(_listenfd, 5);
    if (res < 0) {
        std::cout << "listen failed" << std::endl;
    }
}

void Socket::Accept() {
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    //accept接收客户端连接,注意参数是传入指针,是要求可寻址的,所以不能直接传入sizeof(clnt_addr_size)这种右值
    int clnt_sock = accept(_listenfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");
    
}