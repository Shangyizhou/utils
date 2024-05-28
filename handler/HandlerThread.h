#ifndef HANDLER_HANDLER_THREAD_H
#define HANDLER_HANDLER_THREAD_H

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

#include "Looper.h"

class HandlerThread {
public:
    HandlerThread(const std::string& name);
    ~HandlerThread();

    void threadFunc();

    void quit();

    void quitSafely();

    Looper* getLooper();

private:
    std::string m_name;
    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::atomic<bool> m_exiting;
    std::atomic<bool> m_exited;
    Looper* m_looper;
};

HandlerThread::HandlerThread(const std::string& name)
    : m_name(name),
      m_thread(std::bind(&HandlerThread::threadFunc, this)),
      m_exiting(false),
      m_exited(false),
      m_looper(nullptr) {}

/**
 * 线程的生命周期在HandlerThread的生命周期内
 * HandlerThread销毁
 * 1.让Looper销毁MessageQueue的message
 * 2.Looper持有MessageQueue的unique_ptr，Looper销毁也会被销毁
 * 3.线程结束，则线程静态变量销毁，则t_Looper指针销毁
 * 4.HandlerThread保存了t_Looper的指针到m_looper，此时再销毁
*/
HandlerThread::~HandlerThread() {
    if (m_looper)  {
        m_looper->quitSafely();
    }
    m_thread.join();
    if (m_looper) {
        delete m_looper;
        m_looper = nullptr;
    }
}

void HandlerThread::threadFunc() {
    Looper::prepare();
    m_looper = Looper::getLooper();
    m_cond.notify_all();
    // may block
    m_looper->loop();
    // if message nullptr quit loop
    m_looper->quitSafely();
    m_exiting = false;
    m_exited = true;
    m_looper = nullptr;
}

void HandlerThread::quit() {
    if (m_looper) {
        m_looper->quit();
    }
}

void HandlerThread::quitSafely() {
    if (m_looper) {
        m_looper->quitSafely();
    }
}

Looper* HandlerThread::getLooper() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_looper == nullptr) {
        m_cond.wait(lock, [this](){
            return m_looper != nullptr;
        });
    }
    return m_looper;
}

#endif // HANDLER_HANDLER_THREAD_H