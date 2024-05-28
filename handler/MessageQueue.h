#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "Message.h"

#include <list>
#include <thread>
#include <condition_variable>


class MessageQueue {
public:
    /**
     * Looper::prepare创建的是允许退出的
    */
    MessageQueue(bool is_quit_allowed);

    ~MessageQueue();

    Message* next();

    void quit(bool safe);

    bool enqueueMessage(Message* message);

    void removeMessage(int what);

    void clearAllMessages();

    bool empty();

    int size();

private:
    bool m_is_quit_allowed;
    bool m_is_quiting;
    bool m_is_blocked;
    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cond;
    std::list<Message*> m_queue;
};

#endif // MESSAGE_QUEUE_H