#include <iostream>
using namespace std;

void func1(int& i)
{
    cout << "参数是左值" << endl;
}

void func1(int&& i)
{
    cout << "参数是右值" << endl;
}

/**
 * 如果左值和右值进入了一个函数，然后再被转发怎么办
 * 如果这样，最终都会变成左值，因为有形参的产生，其会有地址
 * 这个时候就需要使用C++的完美转发，可以保留参数的左值或右值特性
*/
template <typename T>
void func3(T&& i) // T&& 可以接收左值和右值
{
    // 完美转发保留了左值引用和右值引用的特性
    func1(std::forward<T>(i));
}

int main()
{
    int i = 3;
    func3(i); // 参数是左值
    func3(3); // 参数是右值
}