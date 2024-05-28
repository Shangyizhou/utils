#include "RedisClient.h"

RedisClient::RedisClient(const std::string& ip, const uint16_t& port) {
    m_context = redisConnect(ip.c_str(), port);
    if (m_context == nullptr || m_context->err) {
        if (m_context) {
            throw connection_error("m_context error");
        } else {
            throw connection_error("can't allocate redis context");
        }
    } else {
        m_connnected = true;
    }
}

/**
 * redisCommand
 * redisvCommand：发送格式化的redis命令
 * redisCommandArgv：设置命令的字符串数量，获取每个字符串以及长度，不为'/0'所阻塞
 * redisAppendCommandArgv: redis利用管道来执行命令
 * 
 * 
*/
Reply RedisClient::RedisCommand(const std::vector<std::string>& commands) {
    std::vector<const char*> argv;
    std::vector<size_t> argvlen;
    argv.reserve(commands.size());
    argvlen.reserve(commands.size());

    for (const auto& it : commands) {
        argv.push_back(it.c_str());
        argvlen.push_back(it.size());
    }

    // redisCommandArgv
    // redisAppendCommandArgv
    redisReply* r = (redisReply*)redisCommandArgv(m_context, commands.size(), argv.data(), argvlen.data());
    if (r->type == REDIS_REPLY_ERROR) {
        throw transport_failure("REDIS_REPLY_ERROR");
    }

    Reply reply(r);
    freeReplyObject(r);

    return reply;
}

void ParserStream() {
    Reply r;
    auto r2 = r.elements();
    // 解析出 mq
    // 解析出 id
    // 解析出数据
    std::string channel = r.elements()[0].elements()[0].str();
    std::string id = r.elements()[0].elements()[1].elements()[1].elements()[0].str();
    auto list = r.elements()[0].elements()[1].elements()[1].elements()[1].elements();
    std::vector<std::string> res;
    for (const auto& i : list) {
        res.push_back(i.str());
    }
}

Reply RedisClient::GetReply() {
    redisReply *r;
    int error = redisGetReply(m_context, reinterpret_cast<void**>(&r));
    if (error != REDIS_OK) {
        throw transport_failure("Reply is no effect");
    }

    std::cout << r->elements << std::endl;
    for (size_t i = 0; i < r->elements; i++) {
        std::cout << r->element[i] << std::endl;
    }

    Reply reply(r);
    freeReplyObject(r);

    if (reply.type() == Reply::type_t::ERROR 
            && (reply.str().find("READONLY") == 0)) {
        throw slave_read_only("GetReply type error");
    }
    return reply;
}

void RedisClient::Set(const std::string& key, const std::string& value) {
    std::vector<std::string> commands;
    commands.reserve(3);
    commands.push_back("SET");
    commands.push_back(key);
    commands.push_back(value);
    Reply r = RedisCommand(commands);
}

Reply RedisClient::Get(const std::string& key) {
    std::vector<std::string> commands;
    commands.reserve(2);
    commands.push_back("GET");
    commands.push_back(key);
    Reply r = RedisCommand(commands);
    return r;
}

/**
 * 在一个loop内获取
*/
void RedisClient::Subscribe(const std::string& channel) {
    std::vector<std::string> commands;
    commands.reserve(2);
    commands.push_back("SUBSCRIBE");
    commands.push_back(channel);
    try {
        Reply r = RedisCommand(commands);
    } catch (const transport_failure& e) {

    }
}

void RedisClient::Publish(const std::string& channel, const std::string& message) {
    std::vector<std::string> commands;
    commands.reserve(3);
    commands.push_back("PUBLISH");
    commands.push_back(channel);
    commands.push_back(message);
    Reply r = RedisCommand(commands);
    
}



struct MsgData {
    long long id;

};

// read channel messages
// 
/**
 * XREAD COUNT 1 BLOCK 0 STREAMS 云岚宗 0-0
 * 
 * COUNT：表示每个流中最多读取的元素个数；
 * BLOCK：阻塞读取，当消息队列没有消息的时候，则阻塞等待， 0 表示无限等待，单位是毫秒。
 * ID：消息 ID，在读取消息的时候可以指定 ID，并从这个 ID 的下一条消息开始读取，0-0 则表示从第一个元素开始读取。
*/
Reply RedisClient::XREAD(const std::string& channel, int timeout)
{
    std::vector<std::string> commands;
    // std::string timeout_str = std::to_string(timeout);
    commands.reserve(6);
    commands.push_back("XREAD");
    commands.push_back("BLOCK");
    commands.push_back("0");
    commands.push_back("STREAMS");
    commands.push_back(channel);
    // commands.push_back("0-0");
    commands.push_back("$");

    /**
     * 移动逻辑过来
    */
    std::vector<const char*> argv;
    std::vector<size_t> argvlen;
    argv.reserve(commands.size());
    argvlen.reserve(commands.size());

    for (const auto& it : commands) {
        argv.push_back(it.c_str());
        argvlen.push_back(it.size());
    }
    
    // redisReply *r = (redisReply*)redisCommand(m_context, "XREAD BLOCK %d STREAMS %s $",timeout,channel.c_str()); 

    redisReply* r = (redisReply*)redisCommandArgv(m_context, commands.size(), argv.data(), argvlen.data());
    if (r->type == REDIS_REPLY_ERROR) {
        throw transport_failure("REDIS_REPLY_ERROR");
    }

    Reply reply(r);
    std::string c = reply.elements()[0].elements()[0].str();
    std::string id = reply.elements()[0].elements()[1].elements()[0].elements()[0].str();
    auto list = reply.elements()[0].elements()[1].elements()[0].elements()[1].elements();
    std::vector<std::string> res;
    for (const auto& i : list) {
        res.push_back(i.str());
    }

    freeReplyObject(r);
    // Reply r = GetReply();
    // return r;

    return reply;
}


void RedisClient::Publish(const std::string channel,std::map<std::string, std::string> field_string_map) {
    std::vector<std::string> commands;
    commands.reserve(3 + field_string_map.size() * 2);
    commands.push_back("XADD");
    commands.push_back(channel);
    commands.push_back("*");
    for (const auto& it : field_string_map) {
        commands.push_back(it.first);
        commands.push_back(it.second);
    }

    /**
     * 移动逻辑过来
    */
    std::vector<const char*> argv;
    std::vector<size_t> argvlen;
    argv.reserve(commands.size());
    argvlen.reserve(commands.size());

    for (const auto& it : commands) {
        argv.push_back(it.c_str());
        argvlen.push_back(it.size());
    }
    
    redisReply* r = (redisReply*)redisCommandArgv(m_context, commands.size(), argv.data(), argvlen.data());
    if (r->type == REDIS_REPLY_ERROR) {
        throw transport_failure("REDIS_REPLY_ERROR");
    }
}

void RedisClient::XgroupCreate(const std::string& channel, const std::string& group) {
    redisReply *r = (redisReply*)redisCommand(m_context, "XGROUP CREATE %s %s 0",channel.c_str(),group.c_str()); 
    if (r == nullptr) {
        throw transport_failure("XADD error");
    }
    Reply reply(r);
}

// XREADGROUP GROUP consumer_group1 consumer2 COUNT 1 BLOCK 0 STREAMS stream_user >
void RedisClient::XgroupRead(const std::string& channel, const std::string& group, const std::string& consumer, const std::string& delivered_id, const std::map<std::string,std::string>& MsgMap, int number) {
    redisReply *r = (redisReply*)redisCommand(m_context, "XREADGROUP GROUP %s %s count %d STREAMS %s >",group.c_str(),consumer.c_str(),number,channel.c_str()); 
        if (r == nullptr) {
        throw transport_failure("XgroupRead error");
    }
    Reply reply(r);
}

void RedisClient::XACK(const std::string& channel,  const std::string& group, const std::string& delivered_id) {

}

/*
//put a meassage map into specific channel
int RedisClient::StreamPublish(const std::string channel,std::map<std::string, std::string> field_string_map)
{
    std::vector<std::string> commands;
    commands.reserve(6);
    commands.push_back("XADD");
    commands.push_back(channel);
    commands.push_back("*");
    

	if (!m_context) return -1;
	std::string field_string;
	std::string command = "XADD";
	std::string format = "*";

	int command_length = field_string_map.size();
	const char **commandList = new const char*[3+2*command_length];
	commandList[0] = command.c_str();
	commandList[1] = channel.c_str();
	commandList[2] = format.c_str();

	size_t *lengthList = new size_t[3+2*command_length];
	lengthList[0] = command.length();
	lengthList[1] = channel.length();
	lengthList[2] = format.length();

	int index = 3;
	for(std::map<std::string,std::string>::const_iterator iter = field_string_map.begin(); iter != field_string_map.end(); ++iter)
	{
		commandList[index] = iter->first.c_str();
		lengthList[index] = iter->first.length();
		index++;
		commandList[index] = iter->second.c_str();
		lengthList[index] = iter->second.length();
		index++;
	}
	redisReply *pRedisReply = (redisReply*)redisCommandArgv(m_context, index, (const char**)commandList, (const size_t*)lengthList); 
    if (pRedisReply == NULL)
        return -1;
    fprintf(stderr, "RedisClient:: publish:%s: type:%d, str:%s\n", channel.c_str(), pRedisReply->type, pRedisReply->str);
	if (pRedisReply->type == REDIS_REPLY_NIL)
    {
        freeReplyObject(pRedisReply);
        return -1;
    }
    else
    {
        freeReplyObject(pRedisReply);
        return 0;
    }	
}

//create a group able to consume messages of a channel
int RedisClient::XgroupCreate(std::string channel, std::string group)
{
	int ret = -1;
	if (!m_context) return -1;
	redisReply *pRedisReply = (redisReply*)redisCommand(m_context, "XGROUP CREATE %s %s 0",channel.c_str(),group.c_str()); 
    if (pRedisReply == NULL)
        return -1;
	else if (strcmp(pRedisReply->str, "OK") != 0)
	{
		fprintf(stderr, "RedisAdapter::Create Group %s of %s ERROR!\n",group.c_str(),channel.c_str());
		freeReplyObject(pRedisReply);
		return -1;
	}
	else
	{
		freeReplyObject(pRedisReply);
		ret = 1;
	}
    return ret;
}

//using consuming group read messages of a channel and put key&value into MsgMap
int RedisClient::XgroupRead(std::string channel, std::string group, std::string consumer, std::string &delivered_id, std::map<std::string,std::string> &MsgMap, int number)
{
	int ret = -1;
	if (!m_context)
		return -1;
	redisReply *pRedisReply = (redisReply*)redisCommand(m_context, "XREADGROUP GROUP %s %s count %d STREAMS %s >",group.c_str(),consumer.c_str(),number,channel.c_str()); 
	fprintf(stderr, "RedisAdapter::Init XREADGROUP GROUP %s %s count %d STREAMS %s %s!\n",group.c_str(),consumer.c_str(),number,channel.c_str(), delivered_id.c_str());
    if (pRedisReply == NULL)
	{
		fprintf(stderr, "RedisAdapter::Init ERROR:%s!\n",m_context->errstr);
		return -1;
	}
	if (pRedisReply->type == REDIS_REPLY_NIL)
	{
		fprintf(stderr, "RedisAdapter::Group %s get messages of %s null,keep waiting!\n",group.c_str(),channel.c_str());
		freeReplyObject(pRedisReply);
        return 0;
	}
	else if (pRedisReply->type != REDIS_REPLY_ARRAY)
    {
		fprintf(stderr, "RedisAdapter::Group %s get messages of %s ERROR,type:%d,reply :%s!\n",group.c_str(),channel.c_str(),pRedisReply->type,pRedisReply->str);
        freeReplyObject(pRedisReply);
        return -1;
    }
	else
	{
		redisReply* msg_struct_0 = pRedisReply->element[0]->element[1];
		fprintf(stderr, "RedisAdapter::Group %s get messages of %s,type:%d.\n",group.c_str(),channel.c_str(),pRedisReply->type);
		std::string deliverkey = "delivered_id";
		delivered_id = msg_struct_0->element[0]->element[0]->str;
		MsgMap[deliverkey] = delivered_id;
		redisReply* msg_struct_1 = msg_struct_0->element[0]->element[1];
		for (int i = 0; i < msg_struct_1->elements; i += 2)
        {
			std::string key = msg_struct_1->element[i]->str;
			std::string value =  msg_struct_1->element[i+1]->str;
			MsgMap[key] = value;
			fprintf(stderr, "RedisAdapter::Group %s get messages of %s,key:%s,value :%s!\n",group.c_str(),channel.c_str(),key.c_str(),value.c_str());
		}
		ret = 1;
	}
    freeReplyObject(pRedisReply);
    return ret;
}

int RedisClient::XACK(std::string channel, std::string group, std::string delivered_id)
{
	int ret = -1;
	if (!m_context) return -1;
	redisReply *pRedisReply = (redisReply*)redisCommand(m_context, "XACK %s %s %s",channel.c_str(),group.c_str(),delivered_id.c_str()); 
    if (pRedisReply == NULL)
        return -1;
	if (pRedisReply->type == REDIS_REPLY_INTEGER)
		ret = pRedisReply->integer;
	else if (pRedisReply->type == REDIS_REPLY_ERROR  && strcmp(pRedisReply->str, "ERR Unknown or disabled command 'XACK'") == 0)
		fprintf(stderr, "RedisAdapter::Group %s ack message %s of %s error, because this message has already been acked!\n",group.c_str(),delivered_id.c_str(),channel.c_str());
	else
		fprintf(stderr, "RedisAdapter::Group %s ack message %s of %s error, type : %d, str:%s!\n",group.c_str(),delivered_id.c_str(),channel.c_str(),pRedisReply->type,pRedisReply->str);
	freeReplyObject(pRedisReply);
    return ret;
}

int RedisClient::NextMsgId(const std::string channel, std::string start, std::string end, int count=1)
{
	int ret = -1;
	if (!m_context) return -1;
	redisReply *pRedisReply = (redisReply*)redisCommand(m_context, "XRANGE %s %s %s COUNT %d",channel.c_str(),start.c_str(),end.c_str(),count); 
    if (pRedisReply == NULL)
        return -1;
	if (pRedisReply->type == REDIS_REPLY_NIL)
	{
		ret = 0;
	}
	else if(pRedisReply->type == REDIS_REPLY_ARRAY)
	{
		ret = atoi(pRedisReply->element[0]->str);
	}
	freeReplyObject(pRedisReply);
    return ret;
}

int RedisClient::XDEL(std::string channel, std::string delivered_id)
{
	int ret = -1;
	if (!m_context) return -1;
	redisReply *pRedisReply = (redisReply*)redisCommand(m_context, "XDEL %s %s",channel.c_str(),delivered_id.c_str()); 
    if (pRedisReply == NULL)
        return -1;
	else if (pRedisReply->type == REDIS_REPLY_INTEGER)
		ret = pRedisReply->integer;
	return ret;
}

int RedisClient::ConsumeMsg(std::string channel, std::string group, std::string consumer, std::string &delivered_id, std::map<std::string,std::string> &MsgMap, int number, int reconn)
{
	int ret = -1;
	int retry = 0;
	while (ret < 0) {
		if (!GetConnected())
		{
			Connect();
		}
		ret = XgroupRead(channel,group,consumer,delivered_id, MsgMap, number);
		if (ret < 0)
		{
			SetConnected(false);
			Close();
		}
		retry++;
		if (retry > reconn)
			break;
	}
	return ret;
}

int RedisClient::DeleteMsg(std::string channel, std::string delivered_id,int reconn)
{
	int ret = -1;
	int retry = 0;
	while (ret < 0) {
		if (!GetConnected())
		{
			Connect();
		}
		ret = XDEL(channel,delivered_id);
		if (ret < 0)
		{
			SetConnected(false);
			Close();
		}
		retry++;
		if (retry > reconn)
			break;
	}
	return ret;
}

int RedisClient::ACK(std::string channel, std::string group, std::string delivered_id,int reconn)
{
	int ret = -1;
	int retry = 0;
	while (ret < 0) {
		if (!GetConnected())
		{
			Connect();
		}
		ret = XACK(channel,group,delivered_id);
		if (ret < 0)
		{
			SetConnected(false);
			Close();
		}
		retry++;
		if (retry > reconn)
			break;
	}
	return ret;
}

*/