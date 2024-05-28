#ifndef MESSAGE_H
#define MESSAGE_H

#include "handler.h"
#include <string>

namespace thread {

class Handler;

class Message {
public:
  Message();

  ~Message();

public:
    int what;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    int arg6;
    int arg7;
    void *obj1;
    void *obj2;

public:
    Handler *target;
};



}  // namespace thread

#endif // MESSAGE_H