#include "Handler.h"
#include "MessageQueue.h"
#include "Looper.h"

Handler::Handler() {

}

Handler::Handler(Looper *looper, HandlerCallback callback) {
    m_handler_message_cb = std::move(callback);
    m_looper = looper;
}

Handler::~Handler() {

}

void Handler::sendMessage(Message *message) {
    if (message->target == nullptr) {
        message->target = this;
    }

    m_looper->sendMessage(message);
}

void Handler::dispatchMessage(Message *message) {
    if (message == nullptr) {
        std::cout << "dispatchMessage but message is nullptr" << std::endl;
        return;
    }

    if (m_handler_message_cb) {
        m_handler_message_cb(message);
    }
}

Message* Handler::obtainMessage(int what) {
    
}

