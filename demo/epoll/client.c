 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
 
 
#define SERVPORT 9527
#define MAXBUF 1024*1024  //1MB
#define MAXFDS 5000
#define EVENTSIZE 100
 
int setnonblocking(int fd)
{
    int opts;
    if( (opts = fcntl(fd, F_GETFL, 0)) == -1)
    {
        perror("fcntl");
        return -1;
    }
 
    opts = opts | O_NONBLOCK;
    if( (opts = fcntl(fd, F_SETFL, opts)) == -1)
    {
        perror("fcntl");
        return -1;
    }
 
    return 0;
}
 
int main(void) {
    int len = 0, n = 0, once = 0;
    int  hasSend;
    char buf[MAXBUF] = {0};
    char sndMsg[MAXBUF] = {1};
 
    struct sockaddr_in servaddr;
    int sockfd, listenfd, epollfd, nfds;
 
    struct epoll_event ev;
    struct epoll_event events[EVENTSIZE];
 
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);
 
    if( (epollfd = epoll_create(MAXFDS)) == -1) {
        perror("epoll");
        exit(1);
    }
 
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
 
    if(setnonblocking(listenfd) == -1) {
        perror("setnonblocking");
        exit(1);
    }
 
    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(1);
    }
 
    if(listen(listenfd, 10) == -1) {
        perror("listen");
        exit(1);
    }
 
 
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenfd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
 
    for( ; ; ) {
        if( (nfds = epoll_wait(epollfd, events, EVENTSIZE, -1)) == -1) {
            perror("epoll_wait");
            exit(1);
        }
 
        for(n = 0; n < nfds; n++) {
            if(events[n].data.fd == listenfd) {
                while( (sockfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) > 0) {
                    if(setnonblocking(sockfd) == -1) {
                        perror("setnonblocking");
                        exit(1);
                    }
                    ev.events = EPOLLIN | EPOLLET;//只注册EPOLLIN
                    ev.data.fd = sockfd;
                    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
                    {
                        perror("epoll_ctl");
                        exit(1);
                    } else {
                        printf("new socketfd = %d register epoll success\n", sockfd);
                    }
                }
                continue;
            }
            printf("Events: 0x%x\n", events[n].events);
            if (events[n].events & EPOLLIN) {
                printf(">>Only EPOLLIN socketfd = %d\n", events[n].data.fd);
                len = recv(events[n].data.fd, buf, 1024, 0);
                if (len == 0) {//因为发生了EPOLLIN事件，所以调用recv函数，但是却返回是0，所以可以认为是对端关闭了socket
                    printf("Peer is closed, then we must close socket....\n");
                    close(events[n].data.fd);
                } else if (len == -1) {
                  printf("len = -1, errno=%d\n", len);
                  exit(-1);
                } else {
                  printf("len = %d, data = %s\n", len, buf);
                }
            }
            if (events[n].events & EPOLLOUT) {
                printf(">> Only EPOLLOUT socketfd = %d\n", events[n].data.fd);
            }
        }
    }
}