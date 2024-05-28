#include <iostream>

template <class T>
class shared_ptr {
public:
    shared_ptr()
        : ptr_(nullptr), count_(nullptr) {}
    shared_ptr(const shared_ptr<T>& other) 
        : ptr_(other.ptr_), count_(new int(*(other.count_) + 1))

private:
    T* ptr_;
    int* count_;
};