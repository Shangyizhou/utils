#include "RedisClient.h"
#include <thread>

void Subscribe() {
    RedisClient client("127.0.0.1", 6379);
    client.Subscribe("channel_1");
    while (true) {
        try {
            auto r = client.GetReply();
            for (const auto& it : r.elements()) {
                std::cout << it.str() << std::endl;
            }
        } catch (RedisException e) {
            std::cout << "catch error" << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
}

void StreamTest() {
    RedisClient client("127.0.0.1", 6379);

    // 连续发送五条信息
    for (int i = 0; i < 10; i++) {
        std::string uid = std::to_string(i);
        std::map<std::string, std::string> map = {
            {"uid", uid},
            {"age", uid} 
        };
        client.Publish("mq_stream", map);
    }

    // 创建两个消费组
    client.XgroupCreate("mq_stream", "consumer_group_1");
    client.XgroupCreate("mq_stream", "consumer_group_2");

    for (int i = 0; i < 5; i++) {
        XgroupRead("mq_stream", "consumer_group_1", "consumer_group_1_1", 1);
    }
    
}

// 连接了docker启动的redis，端口映射6379
int main() {
   StreamTest();
   
   std::this_thread::sleep_for(std::chrono::seconds(600));

    return 0;
}