#ifndef HANDLER_HANDLER_H
#define HANDLER_HANDLER_H

#include <functional>

class MessageQueue;
class Looper;
class Message;

class Handler {
public:
    typedef std::function<void(Message*)> HandlerCallback;

    Handler();

    Handler(Looper *looper);

    /** 
     * 可以在创建handler的时候以lambda表达式方法传入
    */
    Handler(Looper *looper, HandlerCallback callback);

    virtual ~Handler();

    void sendMessage(Message *message);

    void dispatchMessage(Message *message);

    virtual Message* obtainMessage(int what);

private:
    HandlerCallback m_handler_message_cb;
    Looper* m_looper; // 一个looper可以催生出很多个handler，所以looper的生命周期不应由一个handler来决定
};

#endif // 