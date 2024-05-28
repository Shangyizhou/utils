#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char** argv) {
    char message[] = "Hello, world!";

    if (argc != 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket error");
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));
}