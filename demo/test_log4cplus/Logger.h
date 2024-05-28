#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "log4cplus/logger.h"
#include "log4cplus/consoleappender.h"
#include "log4cplus/helpers/appenderattachableimpl.h"
#include "log4cplus/helpers/loglog.h"
#include "log4cplus/helpers/pointer.h"
#include "log4cplus/helpers/property.h"
#include "log4cplus/spi/loggingevent.h"
#include "log4cplus/initializer.h"
#include "log4cplus/loggingmacros.h"

class Logger
{
public:
    // 初始化 log4cplus 的属性
    log4cplus::Logger initLog4cplus();

    // 获取单例 Logger
    static Logger& getInstance();

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;
};

#define TRACE(p)    LOG4CPLUS_TRACE(Logger::getInstance().initLog4cplus(), p)
#define DEBUG(p)    LOG4CPLUS_DEBUG(Logger::getInstance().initLog4cplus(), p)
#define INFO(p)     LOG4CPLUS_INFO(Logger::getInstance().initLog4cplus(), p)
#define WARN(p)     LOG4CPLUS_WARN(Logger::getInstance().initLog4cplus(), p)
#define ERROR(p)    LOG4CPLUS_ERROR(Logger::getInstance().initLog4cplus(), p)
#define FATAL(p)    LOG4CPLUS_FATAL(Logger::getInstance().initLog4cplus(), p)