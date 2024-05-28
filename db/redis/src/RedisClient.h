#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include "hiredis/hiredis.h"
#include "RedisExeception.h"
#include "Reply.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unistd.h>
#include <cstring>

/**
//  * 大类型的错误
// */
// enum RedisError {
//     M_REDIS_OK = 0,           // 执行成功
//     M_CONNECT_FAIL = -1,      // 连接redis失败
//     M_CONTEXT_ERROR = -2,     // RedisContext返回错误
//     M_REPLY_ERROR = -3,       // redisReply错误
//     M_EXE_COMMAND_ERROR = -4, // redis命令执行错误
//     M_NIL_ERROR = -5          // nil
// };

// /**
//  * RedisReply Type
// */
// enum ReplyType {
//     REPLY_INVALID = -1,
//     REPLY_STRING,
//     REPLY_INTEGER,
//     REPLY_ARRAY,
//     REPLY_ERROR,
//     REPLY_NULL,
//     REPLY_OTHER
// };

// struct RedisReply {
//     ReplyType type;
//     size_t inter;
//     std::string data;
//     std::vector<std::string> data_list;
// };

REDIS_EXCEPTION(connection_error)
REDIS_EXCEPTION_2(unable_to_connect, connection_error)
REDIS_EXCEPTION_2(transport_failure, connection_error)
REDIS_EXCEPTION_2(slave_read_only, connection_error)

class RedisClient {
public:
    RedisClient(const std::string& ip, const uint16_t& port);
    
    void Close() { redisFree(m_context); }

    Reply RedisCommand(const std::vector<std::string>& commands);

    Reply GetReply();

    void Set(const std::string& key, const std::string& value);

    Reply Get(const std::string& key);

    void Subscribe(const std::string& channel);

    void Publish(const std::string& channel, const std::string& message);

    bool GetConnected() { return m_connnected; }

    void SetConnected(bool flag) { m_connnected = false; }

    /**
     * stream function
     */
    // int XADD(const std::string channel, const std::map<std::string,std::string> field_string_map);
    Reply XREAD(const std::string& channel, const int timeout=0);
    void Publish(const std::string channel,std::map<std::string, std::string> field_string_map);
    void XgroupCreate(const std::string& channel, const std::string& group);
    void XgroupRead(const std::string& channel, const std::string& group, const std::string& consumer, 
         const std::string& delivered_id, const std::map<std::string,std::string>& MsgMap, int number=1);//异步多消费者消费
    void XACK(const std::string& channel,  const std::string& group, const std::string& delivered_id);
    void NextMsgId(const std::string channel, std::string start, std::string end, int count);//return 待处理
    void XDEL(std::string channel, std::string delivered_id);

    void CreateConsumerGroup(std::string channel,  std::string group, int reconn);
    void ConsumeMsg(std::string channel, std::string group, std::string consumer, std::string &delivered_id, std::map<std::string,std::string> &MsgMap, int number, int reconn);
    void DeleteMsg(std::string channel,std::string delivered_id, int reconn);
    void ACK(std::string channel, std::string group, std::string delivered_id,int reconn);

private:
    redisContext*   m_context;
    redisReply*     m_reply;
    bool            m_connnected;
};

#endif // REDIS_CLIENT_H