#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H

#include <iostream>
#include <string>
#include <functional>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <arpa/inet.h>

class TcpServer {
public:
    typedef std::function<void()> ReadCallback;

    TcpServer(const uint32_t& port);
    ~TcpServer();

    void SetDefault();

    void Accept();

    void SetReadcallback(ReadCallback callback) {
        m_read_callback = callback;
    }

    void SetNonBlock() {
        
    }

private:
    void Bind();
    void Listen();


    int _listenfd;
    std::string _ip;
    uint32_t _port;
    ReadCallback m_read_callback;
};

TcpServer::TcpServer(const uint32_t& port) : _port(port) {
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenfd < 0) {
        std::cout << "socket error" << std::endl;
    }
}

TcpServer::~TcpServer() {
    if (_listenfd < 0) {
        std::cout << "_listenfd error" << std::endl;
        return;
    }
    close(_listenfd);
}

void TcpServer::SetDefault() {
    Bind();
    Listen();
}

void TcpServer::Bind() {
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

void TcpServer::Listen() {
    int res = listen(_listenfd, 10);
    if (res < 0) {
        std::cout << "listen failed" << std::endl;
    }
}

void TcpServer::Accept() {
    while (true) {
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        //accept接收客户端连接,注意参数是传入指针,是要求可寻址的,所以不能直接传入sizeof(clnt_addr_size)这种右值
        int clnt_sock = accept(_listenfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            std::cout << "accept() error" << std::endl;
        }
        char recv[512];
        read(clnt_sock, recv, sizeof(recv));
        std::cout << recv << std::endl;

        char buf[] = "hello world";
        write(clnt_sock, buf, sizeof(buf));
        if (m_read_callback) {
            m_read_callback();
        } 
    }
}

#endif // NET_TCP_SERVER_H