#ifndef REDIS_EXCEPTION_H
#define REDIS_EXCEPTION_H

#include <exception>
#include <string>

class RedisException : public std::exception {
public:
    RedisException(const std::string& what) : m_what(what) {}
    virtual ~RedisException() {}

    virtual const char* what() const throw() { return m_what.c_str(); }


private:
    std::string m_what;
};

#define REDIS_EXCEPTION(name) class name : public RedisException {\
    public: name(const std::string& what = "") : RedisException(what){}};

#define REDIS_EXCEPTION_2(name, super) class name : public super {\
    public: name(const std::string& what = "") : super(what){}};

#endif // REDIS_EXCEPTION_H  