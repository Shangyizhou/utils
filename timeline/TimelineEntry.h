#ifndef TIME_LINE_ENTRY_H
#define TIME_LINE_ENTRY_H

#include "IMessage.h"
#include <inttypes.h>

typedef uint64_t Long;

class TimelineEntry {
public:
    /**
     * 构造函数。
     * @param sequenceID    顺序ID，由LIB或存储系统生成。
     * @param message       消息实体。
     */
    TimelineEntry(Long sequenceID, IMessage* message) {
        this->sequenceID = sequenceID;
        this->message = message;
    }

    /**
     * 读取消息的顺序ID，此ID是严格递增的，由LIB或存储系统提供。不需要使用者设置。
     * @return      消息的顺序ID。
     */
    Long getSequenceID() {
        return sequenceID;
    }

    /**
     * 读取消息实体。
     * @return  消息实体。
     */
    const IMessage* getMessage() const {
        return message;
    }

private:
    /**
     * 顺序（sequence）ID，保证严格递增。由LIB或存储层系统生成。LIB使用者无需创建。
     */
    Long sequenceID;

    /**
     * 消息实体，需要自定义消息类，且实现IMessage接口。
     */
    IMessage* message = nullptr;
};

#endif // TIME_LINE_ENTRY_H