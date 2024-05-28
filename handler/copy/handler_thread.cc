#include "handler_thread.h"
#include "log.h"

namespace thread {


HandlerThread *HandlerThread::Create(std::string name) {
  return new HandlerThread(name);
}

static void *RunTask(void *context) {
  auto handler_thread = reinterpret_cast<HandlerThread *>(context);
  handler_thread->RunInternal();
  pthread_exit(nullptr);
}

HandlerThread::HandlerThread(std::string name)
  : name_(name)
  , looper_(nullptr)
  , exiting_(false)
  , exited_(false) {
  pthread_mutex_init(&mutex_, nullptr);
  pthread_cond_init(&cond_, nullptr);
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&thread_, &attr, RunTask, (void *) this);
}

void HandlerThread::RunInternal() {
  pthread_mutex_lock(&mutex_);
  exiting_ = false;
  exited_ = false;
  pthread_mutex_unlock(&mutex_);

  Looper::Prepare();
  pthread_mutex_lock(&mutex_);
  looper_ = Looper::MyLooper();
  pthread_cond_broadcast(&cond_);
  pthread_mutex_unlock(&mutex_);

  Looper::Loop();
  Looper::Exit();

  pthread_mutex_lock(&mutex_);
  exiting_ = false;
  looper_ = nullptr;
  exited_ = true;
  pthread_mutex_unlock(&mutex_);
}

HandlerThread::~HandlerThread() {
  if (looper_) {
    looper_->Quit(true);
  }
  pthread_join(thread_, nullptr);
  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
  if (looper_) {
    delete looper_;
    looper_ = nullptr;
  }
}

void HandlerThread::Quit() {
  pthread_mutex_lock(&mutex_);
  if (exiting_ || exited_) {
    pthread_mutex_unlock(&mutex_);
    return;
  }
  exiting_ = true;
  pthread_mutex_unlock(&mutex_);
  Looper *looper = GetLooper();
  if (looper) {
    looper->Quit(false);
  }
}

bool HandlerThread::QuitSafely() {
  pthread_mutex_lock(&mutex_);
  if (exiting_ || exited_) {
    pthread_mutex_unlock(&mutex_);
    return false;
  }
  exiting_ = true;
  pthread_mutex_unlock(&mutex_);
  Looper *looper = GetLooper();
  if (looper) {
    looper->Quit(true);
    return true;
  }
  return false;
}

Looper *HandlerThread::GetLooper() {
  pthread_mutex_lock(&mutex_);
  if (exited_) {
    LOGE("Thread has been exited");
    pthread_mutex_unlock(&mutex_);
    return nullptr;
  }
  if (looper_ == nullptr) {
    LOGE("Thread should wait");
    pthread_cond_wait(&cond_, &mutex_);
  }
  pthread_mutex_unlock(&mutex_);
  return looper_;
}