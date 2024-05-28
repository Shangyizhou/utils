#include "message_queue.h"
#include "log.h"
#include <sstream>

namespace thread {

MessageQueue::MessageQueue()
  : is_destroyed_(false) {
  pthread_mutex_init(&queue_mutex_, nullptr);
  pthread_cond_init(&queue_cond_, nullptr);
}

MessageQueue::~MessageQueue() {
  LOGI("Enter");
  pthread_mutex_lock(&queue_mutex_);
  is_destroyed_ = true;
  pthread_mutex_unlock(&queue_mutex_);

  Clear();

  pthread_mutex_destroy(&queue_mutex_);
  pthread_cond_destroy(&queue_cond_);
  LOGI("Leave");
}

void MessageQueue::Offer(Message *msg) {
  pthread_mutex_lock(&queue_mutex_);
  if (is_destroyed_) {
    pthread_mutex_unlock(&queue_mutex_);
    return;
  }
  queue_.push_back(msg);
  pthread_cond_broadcast(&queue_cond_);
  pthread_mutex_unlock(&queue_mutex_);
}

void MessageQueue::OfferAtFront(Message *msg) {
  pthread_mutex_lock(&queue_mutex_);
  if (is_destroyed_) {
    pthread_mutex_unlock(&queue_mutex_);
    return;
  }
  queue_.push_front(msg);
  pthread_cond_broadcast(&queue_cond_);
  pthread_mutex_unlock(&queue_mutex_);
}

Message *MessageQueue::Take() {
  pthread_mutex_lock(&queue_mutex_);
  if (is_destroyed_) {
    pthread_mutex_unlock(&queue_mutex_);
    return nullptr;
  }
  if (Size() <= 0) {
    pthread_cond_wait(&queue_cond_, &queue_mutex_);
  }
  if (queue_.empty()) {
    pthread_mutex_unlock(&queue_mutex_);
    return nullptr;
  }
  Message *msg = queue_.front();
  queue_.pop_front();
  pthread_mutex_unlock(&queue_mutex_);
  return msg;
}

void MessageQueue::Notify() {
  pthread_mutex_lock(&queue_mutex_);
  pthread_cond_broadcast(&queue_cond_);
  pthread_mutex_unlock(&queue_mutex_);
}

int MessageQueue::Size() {
  return queue_.size();
}

bool MessageQueue::IsEmpty() {
  return queue_.empty();
}

void MessageQueue::Clear() {
  Notify();
  if (queue_.empty()) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  while (!queue_.empty()) {
    Message *msg = queue_.front();
    queue_.pop_front();
    if (msg) {
      delete msg;
    }
  }
  queue_.clear();
  pthread_mutex_unlock(&queue_mutex_);
}

void MessageQueue::RemoveMessage(int what) {
  pthread_mutex_lock(&queue_mutex_);
  if (is_destroyed_) {
    pthread_mutex_unlock(&queue_mutex_);
    return;
  }
  std::list<Message *>::iterator it = queue_.begin();
  while (it != queue_.end()) {
    Message *msg = *it;
    if (what == msg->what) {
      delete msg;
      it = queue_.erase(it);
      continue;
    }
    ++it;
  }
  pthread_mutex_unlock(&queue_mutex_);
}

void MessageQueue::Dump() {
  std::ostringstream os;
  std::list<Message *>::iterator it = queue_.begin();
  while (it != queue_.end()) {
    Message *msg = *it;
    os << msg->what<<"\n";
    ++it;
  }
  LOGI("Result=%s", os.str().c_str());
}

}