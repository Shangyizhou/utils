#include "Logger.h"

Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}

log4cplus::Logger Logger::initLog4cplus()
{
    // 开启调试信息
    // log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(true);
 
    // 创建输出到终端的appender
    log4cplus::SharedAppenderPtr appenderConsole(new log4cplus::ConsoleAppender(false, true));
    appenderConsole->setName(LOG4CPLUS_TEXT("Console"));
    
    // 设置appender的输出格式
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y/%m/%d %H:%M:%S,%q} [%-5t] [%-5p] - %m%n");
    appenderConsole->setLayout(std::unique_ptr<log4cplus::Layout>(
        new log4cplus::PatternLayout(pattern)));
    
    // getInstance() 返回 static log4cplus::Logger;
    auto log4 = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("normal"));
    log4.addAppender(appenderConsole);

    return log4;
}