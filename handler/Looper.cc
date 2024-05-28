#include "Looper.h"
#include "Handler.h"
#include "MessageQueue.h"

thread_local Looper* t_looper = nullptr;

Looper::Looper(bool is_quit_allowed) {
    m_message_queue = new MessageQueue(is_quit_allowed);
    m_is_exiting = false;
    m_is_looping = false;
}

Looper::~Looper() {
    delete m_message_queue;
}

// 一个线程，只能调用一次 prepare
// 一个线程只能有一个 Looper
void Looper::prepare() {
    if (t_looper != nullptr) {
        std::cout << "Looper existed in thread" << std::endl;
        return;
    }
    t_looper = (new Looper(true));
}

void Looper::loop() {
    Looper* loop = getLooper();
    if (loop == nullptr) {
        std::cout << "Looper is null, not permission to start loop" << std::endl;
        return;
    }

    if (m_is_looping || m_is_exiting) {
        std::cout << "m_is_looping || m_is_exiting || m_exited" << std::endl;
        return;
    }

    m_is_looping = true;

    for (;;) {
        if (m_is_exiting) {
            break;
        }

        // might block
        Message* message = m_message_queue->next();
        if (message == nullptr) {
            std::cout << "In loop. but message is null"  << std::endl;
            return;
        }
        
        if (message->target == nullptr) {
            delete message;
        }
        
        // 知道这个message是哪个handler发出的
        // 在looper所在线程执行handler处重写的回调
        Handler *handler = message->target;
        handler->dispatchMessage(message);
    }
}

Looper* Looper::getLooper() {
    if (t_looper != nullptr) {
        return t_looper;
    }
    return nullptr;
}

void Looper::quit() {
    if (m_is_exiting) {
        return;
    }
    m_is_exiting = true;
    m_message_queue->quit(false);
}

void Looper::quitSafely() {
    m_is_exiting = true;
    m_message_queue->quit(true);
}

int Looper::size() {
    return m_message_queue->size();
}

void Looper::sendMessage(Message* message) {
    if (m_is_exiting) {
        return;
    }
    m_message_queue->enqueueMessage(message);
}

void Looper::removeMessage(Message* message) {

}