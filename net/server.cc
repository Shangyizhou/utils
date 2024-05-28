#include "TcpServer.h"

int main() {
    TcpServer server(3300);
    server.SetDefault();
    server.Accept();

    return 0;
}