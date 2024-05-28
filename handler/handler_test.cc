#include "HandlerThread.h"
#include "Handler.h"
#include "Message.h"
#include <thread>

enum EventType {
    UPDATE_EVENT = 0,
    SUBSCRIVE_EVENT
};

void handleMessage(Message* message) {
    std::cout << std::this_thread::get_id() << std::endl;
    switch (message->what) {
        case UPDATE_EVENT:
            std::cout << "Update Event" << std::endl;
            break;
        case SUBSCRIVE_EVENT:
            std::cout << "Subscribed Event" << std::endl;
            break;
        default:
            std::cout << "Unknown Event" << std::endl;
            break;
    }
}

int main() {
    HandlerThread thread("handler");
    Handler handler(thread.getLooper(), handleMessage); // 这个线程loop会一直阻塞，导致HandlerThread释放也被阻塞
    Message* message = new Message;
    message->what = UPDATE_EVENT;
    handler.sendMessage(message);
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << std::this_thread::get_id() << std::endl;
    thread.quitSafely();

    return 0;
}