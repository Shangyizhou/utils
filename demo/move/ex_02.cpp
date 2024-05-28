#include <iostream>
using namespace std;

/**
 * 为什么要使用右值引用：可以使用右值引用实现移动语义，即实现移动构造函数和移动赋值运算符、
 * 比如堆区的资源需要实现深拷贝，那么就会创建一个新的对象，然后将值拷贝到新对象
 * 但是我们可能传入一个临时对象，然后拷贝销毁。这样不如直接使用这个临时对象
 *  1.就可以省去构造新变量的开销
 *  2.复制的开销
 *  3.析构临时变量的开销
 * 
*/

class Foo
{
public:
    Foo() : ptr(new int(0)) { }
    ~Foo() { delete ptr; }

    // 拷贝构造函数
    // 如果数据很大，申请内存和拷贝操作开销很大
    Foo(const Foo& foo)
    {
        if (!ptr) delete ptr;
        ptr = new int(*(foo.ptr));
        cout << "拷贝构造函数" << endl;
    }

    // 移动构造函数
    Foo(Foo&& foo)
    {
        cout << "移动构造函数" << endl;
        if (!ptr) delete ptr;
        ptr = foo.ptr;
        foo.ptr = nullptr;
    }

    // 赋值运算符
    // 如果数据很大，拷贝操作开销很大
    Foo& operator=(const Foo& foo) 
    {
        cout << "拷贝赋值运算符" << endl;
        if (!ptr) delete ptr;
        ptr = new int(*(foo.ptr));
        return *this;
    }

    Foo& operator=(Foo&& foo)
    {
        cout << "移动赋值运算符" << endl;
        if (!ptr) delete ptr;
        ptr = foo.ptr;
        foo.ptr = nullptr;
        return *this;
    }

private:
    int* ptr;
};

Foo getFoo()
{
    cout << "getFoo" << endl;
    return Foo();
}

int main()
{
    Foo foo;

    Foo foo2 = foo; // 拷贝构造函数

    Foo foo3;
    foo3 = foo; // 拷贝赋值运算符

    cout << "---" << endl;
    Foo foo4; 
    foo4 = getFoo(); // 移动构造函数(getFoo返回临时值) + 移动赋值运算符（临时变量赋值运算）

    /**
     * 我们可以使用 std::move 将左值转换为右值，然后调用移动语义，不过原左值的值就消失了
     * C++11 所以容器都实现了移动语义，避免对含有资源的对象发生无谓的拷贝
     * 移动语义对拥有资源（如内存，文件句柄）的对象有效
     */
    

    return 0;
}