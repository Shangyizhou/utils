#include <iostream>
#include <string>

using namespace std;

class Integer {
public:
    //参数为常量左值引用的深拷贝构造函数，不改变 source.ptr_ 的值
    Integer(const Integer& source)
      : ptr_(new int(*source.ptr_)) {
        cout << "Call Integer(const Integer& source)" << endl;
    }
    
    //参数为左值引用的深拷贝构造函数，转移堆内存资源所有权，改变 source.ptr_ 的值
    Integer(Integer& source)
      : ptr_(source.ptr_) {
        source.ptr_ = nullptr;
        cout << "Call Integer(Integer& source)" << endl;
    }
    
    //移动构造函数，与参数为左值引用的深拷贝构造函数基本一样
    Integer(Integer&& source)
      : ptr_(source.ptr_) {
        source.ptr_ = nullptr;
        cout << "Call Integer(Integer&& source)" << endl;
    }

    Integer(int value)
      : ptr_(new int(value)) {
        cout << "Call Integer(int value)" << endl;
    }

    ~Integer() {
        cout << "Call ~Integer()" << endl;
        delete ptr_;
    }

    int GetValue(void) { return *ptr_; }

private:
    string name_;
    int* ptr_;
};

int main(int argc, char const* argv[]) {

    Integer a(Integer(100));
    int a_value = a.GetValue();
    cout << a_value << endl;
    cout << "-----------------" << endl;
    Integer temp(10000);
    Integer b(temp);
    int b_value = b.GetValue();
    cout << b_value << endl;
    cout << "-----------------" << endl;

    return 0;
}
