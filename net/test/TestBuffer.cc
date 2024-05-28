#include "../Buffer.h"
#include <iostream>

char buf[] = "hello world";

int main() {
    Buffer input_buffer;
    input_buffer.append(buf);
    std::string res = input_buffer.retrieveAllAsString();
    std::cout << res << std::endl;

    return 0;
}