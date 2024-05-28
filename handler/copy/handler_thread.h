#include <pthread.h>
#include <string>
#include "handler.h"
#include "looper.h"

namespace thread {

/**
 * Create -> HandlerThread -> thread RunTask(传入 this)
 * -> RunInternal 
 * -> exiting = false exited = false loop
 * -> exiting_ = false; exited_ = true;
 * ~HandlerThread -> Quit exiting_ = true; -> looper->quit
 * QuitSafely 等待 looper 消费完数据再销毁
*/
class HandlerThread {
public:
  static HandlerThread *Create(std::string name);

  void RunInternal();

private:
  HandlerThread(std::string name);

public:
  ~HandlerThread();

  void Quit();

  bool QuitSafely();

  Looper *GetLooper();

private:
  std::string name_;
  pthread_t thread_;
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
  Looper *looper_;
  bool exiting_;
  bool exited_;
};

}  // namespace thread