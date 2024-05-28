#include "looper.h"
#include "message.h"

namespace thread {

class Looper;
class Message;

/**
 * 消息处理回调
 */
class HandlerCallback {
public:
  virtual void HandleMessage(Message *msg) {}
};

class Handler {
public:
  Handler(Looper *looper, HandlerCallback *callback);

  ~Handler();

  /**
   * 发送消息
   * @param msg
   */
  void SendMessage(Message *msg);

  /**
   * looper中调用此方法回调
   * @param msg
   */
  void DispatchMessage(Message *msg);

  /**
   * 销毁消息队列中msg.what = what的消息
   * @param what
   */
  void RemoveMessage(int what);

  /**
   * 消息队列中消息数
   * @return
   */
  int Size();

private:
  Looper *looper_;
  HandlerCallback *callback_;
};

}  // namespace thread