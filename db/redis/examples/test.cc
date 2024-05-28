#include "hiredis/hiredis.h"
#include <vector>
#include <iostream>

int main() {
    redisContext* m_context = redisConnect("127.0.0.1", 6379);

    std::string s1 = "SET";
    std::string s2 = "Foo";
    std::string s3 = "HH";
    std::vector<std::string> commands;
    commands.reserve(3);
    commands.push_back(s1);
    commands.push_back(s2);
    commands.push_back(s3);


    std::vector<const char*> argv;
    std::vector<size_t> argvlen;
    argv.reserve(commands.size());
    argvlen.reserve(commands.size());

    for (const auto& it : commands) {
        argv.push_back(it.c_str());
        argvlen.push_back(it.size());
    }


    redisReply* r = (redisReply*)redisCommandArgv(m_context, 3, argv.data(), argvlen.data());
    
}