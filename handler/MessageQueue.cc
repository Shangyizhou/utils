#include "MessageQueue.h"
#include <iostream>

MessageQueue::MessageQueue(bool is_quit_allowed) 
    : m_is_quit_allowed(is_quit_allowed),
      m_is_quiting(false),
      m_is_blocked(true) {
    //   
}

MessageQueue::~MessageQueue() {

}

void MessageQueue::clearAllMessages() {
    while (!m_queue.empty()) {
        // new message，所以需要手动delete
        Message* message = m_queue.front();
        // message
        m_queue.pop_front();
        delete message;
    }
}

void MessageQueue::quit(bool safe) {
    if (!m_is_quit_allowed) {
        std::cout << "no permission to quit" << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(m_queue_mutex);
    if (m_is_quiting) {
        return;
    }
    m_is_quiting = true;

    if (safe) {
        // 安全的，移除未来（当前时刻以后）的所有消息
        // 改成执行完当前所有的message，之后不能再加入了
        clearAllMessages();
    } else {
        // 不安全的，移除所有消息。但是会移除内存
        clearAllMessages();
    }
}

bool MessageQueue::empty() {
    std::lock_guard<std::mutex> lock(m_queue_mutex);
    return m_queue.empty();
}

int MessageQueue::size() {
    std::lock_guard<std::mutex> lock(m_queue_mutex);
    return m_queue.size();
}

Message* MessageQueue::next() {
    for (;;) {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_queue_cond.wait(lock, [this](){
            return !m_queue.empty();
        });

        // 跳过那些message不为空，target为空的
        Message* prev_message = nullptr;
        Message* message = nullptr;
        do {
            prev_message = message;
            message = m_queue.front();
            m_queue.pop_front();
            if (prev_message != nullptr) {
                delete prev_message;
            }
        } while (message != nullptr && message->target == nullptr && !m_queue.empty());   

        return message;
    }
}

bool MessageQueue::enqueueMessage(Message* message) {
    if (message->target == nullptr) {
        std::cout << "try to enqueue message, but no target is nullptr" << std::endl;
        return false;
    }

    std::lock_guard<std::mutex> lock(m_queue_mutex);
    if (m_is_quiting) {
        // TODO:回收消息
        // 正在销毁message，不能再加入
        return false;
    }
    m_queue.push_back(message);
    
}