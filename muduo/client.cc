#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpClient.h"
#include <vector>
#include <unordered_map>
#include <string>
using namespace muduo;
using namespace muduo::net;

TcpClient* g_client;

void timeout()
{
  LOG_INFO << "timeout";
  g_client->stop();
}

class Channel {

public:
    // 经过订阅后，通过Register获取IP_List，然后开始连接生成Channel
    void Connect(const InetAddress& server_address) {
        std::unique_ptr<TcpClient> client(new TcpClient(m_loop, server_address, "TcpClient"));
        m_channels.push_back(std::move(client));

    }

private:
    typedef std::vector<std::unique_ptr<TcpClient>> ChannelList;

    EventLoop* m_loop;
    std::vector<std::unique_ptr<TcpClient>> m_channels;
    std::unordered_map<std::string, ChannelList> m_map;
};

int main(int argc, char* argv[])
{
  EventLoop loop;
  InetAddress serverAddr("127.0.0.1", 2); // no such server
  TcpClient client(&loop, serverAddr, "TcpClient");
  g_client = &client;
  loop.runAfter(0.0, timeout);
  loop.runAfter(1.0, std::bind(&EventLoop::quit, &loop));
  client.connect();
  CurrentThread::sleepUsec(100 * 1000);
  loop.loop();
}
