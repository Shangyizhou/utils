#include "looper.h"
#include "thread.h"
#include "log.h"
#include <cassert>
#include "time_utils.h"

namespace thread {

Looper::Looper()
  : exiting_(false)
  , exited_(false)
  , exit_safely_(false)
  , looping_(false) {
  message_queue_ = new MessageQueue();
  pthread_mutex_init(&variable_mutex_, nullptr);
}

Looper::~Looper() {
  pthread_mutex_destroy(&variable_mutex_);
}

void Looper::Prepare() {
  int64_t tid = Thread::CurrentThreadId();
  Looper *looper = LooperManager::GetInstance()->Create(tid);
  if (looper == nullptr) {
    LOGE("Current thread looper has been called");
  }
}

void Looper::Loop() {
  MyLooper()->LoopInternal();
}

Looper * Looper::MyLooper() {
  int64_t tid = Thread::CurrentThreadId();
  Looper *looper = LooperManager::GetInstance()->Get(tid);
  if (looper == nullptr) {
    LOGE("Please invoke Looper::Prepare first");
  }
  assert(looper);
  return looper;
}

int64_t Looper::MyLooperId() {
  return reinterpret_cast<int64_t>(MyLooper());
}

void Looper::Exit() {
  int64_t tid = Thread::CurrentThreadId();
  LooperManager::GetInstance()->Remove(tid);
}

void Looper::Quit(bool safely) {
  pthread_mutex_lock(&variable_mutex_);
  if (exiting_ || exited_) {
    pthread_mutex_unlock(&variable_mutex_);
    return;
  }
  exit_safely_ = safely;
  exiting_ = true;
  pthread_mutex_unlock(&variable_mutex_);
  message_queue_->Notify();
}

void Looper::Dump() {
  message_queue_->Dump();
}

int Looper::Size() {
  return message_queue_->Size();
}

void Looper::SendMessage(Message *msg) {
  pthread_mutex_lock(&variable_mutex_);
  if (exiting_ || exited_) {
    pthread_mutex_unlock(&variable_mutex_);
    return;
  }
  pthread_mutex_unlock(&variable_mutex_);
  EnqueueMessage(msg);
}

void Looper::RemoveMessage(int what) {
  message_queue_->RemoveMessage(what);
}

void Looper::LoopInternal() {
  pthread_mutex_lock(&variable_mutex_);
  if (looping_ || exiting_ || exited_) {
    pthread_mutex_unlock(&variable_mutex_);
    return;
  }
  looping_ = true;
  pthread_mutex_unlock(&variable_mutex_);

  for (;;) {
    Message *msg = Take();
    if (msg) {
      if (msg->target) {
        msg->target->DispatchMessage(msg);
      }
      delete msg;
    }

    pthread_mutex_lock(&variable_mutex_);
    if (exit_safely_) {
      if (exiting_ && message_queue_->Size() == 0) {
        pthread_mutex_unlock(&variable_mutex_);
        break;
      }
    } else {
      if (exiting_) {
        pthread_mutex_unlock(&variable_mutex_);
        break;
      }
    }
    pthread_mutex_unlock(&variable_mutex_);
  }

  int64_t time = TimeUtils::GetCurrentTimeUs();
  while (message_queue_->Size() > 0) {
    Message *msg = message_queue_->Take();
    if (msg) {
      delete msg;
    }
  }
  message_queue_->Clear();
  LOGI("Clear message_queue cost time=%lld us", (TimeUtils::GetCurrentTimeUs() - time));

  pthread_mutex_lock(&variable_mutex_);
  exiting_ = false;
  exited_ = true;
  looping_ = false;
  pthread_mutex_unlock(&variable_mutex_);
}

void Looper::EnqueueMessage(Message *msg) {
  /// TODO msg 模式, 可以放在队头, 也可以放在队尾
  message_queue_->Offer(msg);
}

Message * Looper::Take() {
  return message_queue_->Take();
}

/// ------------------------------------------------------------------

LooperManager *LooperManager::instance_ = new LooperManager();

LooperManager::LooperManager() {

}

LooperManager::~LooperManager() {

}

LooperManager * LooperManager::GetInstance() {
  return instance_;
}

Looper * LooperManager::Create(int64_t tid) {
  std::lock_guard<std::mutex> guard(looper_mutex_);
  auto it = looper_map_.find(tid);
  if (it == looper_map_.end()) {
    Looper *looper = new Looper();
    looper_map_[tid] = looper;
    return looper;
  }
  return nullptr;
}

Looper * LooperManager::Get(int64_t tid) {
  std::lock_guard<std::mutex> guard(looper_mutex_);
  auto it = looper_map_.find(tid);
  if (it == looper_map_.end()) {
    return nullptr;
  }
  return it->second;

}

void LooperManager::Remove(int64_t tid) {
  std::lock_guard<std::mutex> guard(looper_mutex_);
  auto it = looper_map_.find(tid);
  if (it != looper_map_.end()) {
    looper_map_.erase(it);
  }
}

int LooperManager::Size() {
  std::lock_guard<std::mutex> guard(looper_mutex_);
  return looper_map_.size();
}

}