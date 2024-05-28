#include <iostream>
#include "TcpClient.h"

int main() {
    TcpClient client;
    client.Connect("127.0.0.1", 3300);
    client.Send("I am a client");
    std::cout << client.Recv() << std::endl;

    return 0;
}