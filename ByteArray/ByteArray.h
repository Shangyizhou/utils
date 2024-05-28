#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

class ByteArray {
public:
    /**
     * @brief ByteArray的存储节点
     */
    struct Node {
        /**
         * @brief 构造指定大小的内存块
         * @param[in] s 内存块字节数
         */
        Node(size_t s);

        /**
         * 无参构造函数
         */
        Node();

        /**
         * 析构函数,释放内存
         */
        ~Node();

        /// 内存块地址指针
        char* ptr;
        /// 下一个内存块地址
        Node* next;
        /// 内存块大小
        size_t size;
    };


    /**
     * @brief 使用指定长度的内存块构造ByteArray
     * @param[in] base_size 内存块大小
     */
    ByteArray(size_t base_size = 4096);

    /**
     * @brief 析构函数
     */
    ~ByteArray();

    
private:
    /// 内存块的大小
    size_t m_baseSize;
    /// 当前操作位置
    size_t m_position;
    /// 当前的总容量
    size_t m_capacity;
    /// 当前数据的大小
    size_t m_size;
    /// 字节序,默认大端
    int8_t m_endian;
    /// 第一个内存块指针
    Node* m_root;
    /// 当前操作的内存块指针
    Node* m_cur;
};

#endif // BYTE_ARRAY_H