#ifndef HANDLER_MESSAGE_H
#define HANDLER_MESSAGE_H

class Handler;
class Message {
public:
    Message();

    ~Message();

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

    Handler *target;
};

#endif // HANDLER_MESSAGE_H