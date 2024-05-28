#ifndef NET_m_bufferH
#define NET_m_bufferH

#include <vector>
#include <string>
#include <algorithm>

class Buffer {
public:
    static const size_t kCheapPrepend = 8;   // 预留字段，方便在Buffer头加入固定信息，比如消息体长度
    static const size_t kInitialSize = 1024; // Buffer初始化容量

    explicit Buffer(size_t initialSize = kInitialSize)
        : m_buffer(kCheapPrepend + initialSize),
          m_reader_index(kCheapPrepend),
          m_writer_index(kCheapPrepend) {}
    
    size_t readableBytes() const { 
        return m_writer_index - m_reader_index; 
    }
    
    size_t writableBytes() const { 
        return m_buffer.size() - m_writer_index; 
    }
    
    size_t prependableBytes() const { 
        return m_reader_index; 
    }

    /** 
     * 返回缓冲区可读首指针
    */
    const char* beginRead() const {
        return begin() + m_reader_index;
    }
    
    /** 
     * 返回缓冲区可写首指针
    */
    char* beginWrite() {
        return begin() + m_writer_index;
    }

    /**
     * 确保写空间足够，否则扩容
    */
    void ensureWritableBytes(size_t len) {
        if (writableBytes() < len) {
            makeSpace(len);
        }
    }

    /**
     * 说明读指针已经赶上了写指针
     * 没有可读数据了，直接置为开始状态，防止写空间不够
    */
    void retrieveAll() {
        m_reader_index = kCheapPrepend;
        m_writer_index = kCheapPrepend;
    }
    
    /**
     * 读缓冲区数据被取出，读指针向后移动len长度
     * 有时候主动调用write向fd写入数据，写入成功主动调用retrieve方法置位
    */
    void retrieve(size_t len) {
        if (len < readableBytes()) {
            m_reader_index += len;
        } else {
            retrieveAll();
        }
    }

    /**
     * 取出len长度读缓冲区数据，返回String
    */
    std::string retrieveAsString(size_t len) {
        std::string result(beginRead(), len);
        retrieve(len); 
        return result;
    }

    /**
     * 取出所有读缓冲区数据，返回String
    */
    std::string retrieveAllAsString() {   
        return retrieveAsString(readableBytes());
    }

    /**
     * 向Buffer写缓冲区写入数据，从fd读取数据时候会调用
    */
    void append(const char* data, size_t len) {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        m_writer_index += len;
    }

    /**
     * 支持字符串的吸入
    */
    void append(const std::string &str) {
        append(str.data(), str.size());
    }

    /**
     * 从fd上读取数据
    */
    ssize_t readFd(int fd, int *saveErrno);

    /**
     * 通过fd发送数据
    */
    ssize_t writeFd(int fd, int *saveErrno);

private:
    char* begin() {
        return &(*m_buffer.begin());
    }

    const char* begin() const {
        return &(*m_buffer.begin());
    }

    /**
     * 扩容函数
     * 双缓冲区，如果读和写指针不断地后移动，就会导致缓冲区空间缩小，这个时候会有两个判断
     * 1.读缓冲区数据被读取，读指针后移，读指针之前的数据没有利用到，可以向前拷贝
     * 2.算上读指针之前的数据都不够，则申请更多的空间
    */
    void makeSpace(int len) {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
            m_buffer.resize(m_writer_index + len);
        } else { // 整个buffer够用，将后面移动到前面继续分配
            size_t readable = readableBytes();
            std::copy(begin() + m_reader_index,
                    begin() + m_writer_index,
                    begin() + kCheapPrepend);
            m_reader_index = kCheapPrepend;
            m_writer_index = m_reader_index + readable;
        }
    }

    std::vector<char> m_buffer;
    size_t m_reader_index;
    size_t m_writer_index;
};

#endif // NET_m_bufferH