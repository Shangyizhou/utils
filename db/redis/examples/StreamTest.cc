#include "RedisClient.h"
#include <thread>
#include <unordered_map>

void Subscribe() {
    RedisClient client("127.0.0.1", 6379);
    client.Connect();
    client.XREAD("channel2", 0);
}

int main() {
    RedisClient client("127.0.0.1", 6379);
    client.Connect();

    std::string arg1 = "name";
    std::string arg2 = "zhang san";
    std::string arg3 = "age";
    std::string arg4 = "23";
    std::map<std::string,std::string> field(
        {{arg1, arg2}, {arg3, arg4}});
    
    // XADD
    client.StreamPublish("channel2", field);

    std::thread t(Subscribe);
    t.join();

    return 0;
}