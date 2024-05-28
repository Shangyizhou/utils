#ifndef HANDLER_LOOPER_H
#define HANDLER_LOOPER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <memory>

class Message;
class MessageQueue;

class Looper {
public:
    Looper(bool is_quit_allowed);
    ~Looper();

    /**
     * 返回当前线程的looper
    */
    static Looper* getLooper();

    /**
     * 为当前线程准备looper
    */
    static void prepare();

    /**
     * 死循环内取出messageQueue消息
    */
    void loop();

    /**
     * 不安全的退出，不管messageQueue的m剩余essage
     * 但是它会删除消息
    */
    void quit();

    /**
     * 将messageQueue的message处理完后再退出
    */
    void quitSafely();

    int size();

    void sendMessage(Message* message);

    void removeMessage(Message* message);

private:
    std::atomic<bool> m_is_exiting;
    // std::atomic<bool> m_exited;
    std::atomic<bool> m_is_looping;
    std::condition_variable m_cond;
    MessageQueue* m_message_queue;
};

#endif // HANDLER_LOOPER_H