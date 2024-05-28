#ifndef NET_TCP_CLIENT_H
#define NET_TCP_CLIENT_H

#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <arpa/inet.h>

class TcpClient {
public:
    TcpClient() {
        m_connfd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_connfd < 0) {
            std::cout << "create socket error" << std::endl;
        }
    }
    
    ~TcpClient() {
        close(m_connfd);
        std::cout << "TcpClient closed" << std::endl;
    }

    void Connect(const std::string& ip, const uint32_t& port) {
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = inet_addr(ip.c_str());
        int res = connect(m_connfd, (struct sockaddr *)&sin, sizeof(sin));
        if (res != 0) {
            std::cout << "Error connecting to server: " << std::endl;
        }
    }

    void Send(const std::string& str) {
        write(m_connfd, str.c_str(), str.size());
    }

    std::string Recv() {
        char buf[512];
        read(m_connfd, buf, sizeof(buf));
        return buf;
    }
    
private:
    int m_connfd;
    std::string m_ip;
    uint32_t m_port;
    char m_send_buf[512];
    char m_recv_buf[512];
};

#endif // NET_TCP_CLIENT_H