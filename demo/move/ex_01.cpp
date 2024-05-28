#include <iostream>
#include <string>
using namespace std;

/**
 * 1. 什么是左值和右值
 * 左值：可以找到表达式地址，比如 int a = b;
 * 右值：无法找到表达式地址，比如 
 *  非引用返回的临时变量：int a = getNum(); getNum()表达式就是个右值，无法取地址
 *  将亡值：与右值引用相关的表达式，例如被移动的对象，T&&函数返回的值 int a = std::move(b); 
*/
class Foo
{
public:
    Foo() = default;
    ~Foo() = default;

private:
    int num;
};

Foo getFoo()
{
    return Foo();
}

int i = 3; // i是左值，3是右值
int j = i + 3; // j是左值，i + 3是右值
Foo foo = getFoo(); // foo是左值，getFoo的返回值是右值

/**
 * 2.左值引用和右值引用
 * 右值引用：&&
*/

int main()
{
    int&& a = 3; // 3是右值，a被赋值之后成为一个普通变量，其是能够被取值的
    // int&& e = a; 错误的赋值，此时a可以被取地址，a是左值不能使用右值引用
    int b = 8; // b是左值
    int&& c = b + 5; // B + 5是右值

    // getFoo返回的临时变量的生命周期得到了延长
    // return Foo() 生成一个变量，然后return会拷贝操作生成临时变量
    // 临时变量赋值给foo之后就被销毁了
    // 这里使用了右值引用，直接然临时变量作为foo
    // 经典的例子，把A冰箱的大象赋值到B冰箱里如何操作
    // 我们可以不必再创建一个冰箱，然后将大象再复制过去
    // 我们可以把A冰箱的贴纸换成B冰箱，然后使用即可
    Foo&& foo = getFoo(); // getFoo返回值是右值
    
}