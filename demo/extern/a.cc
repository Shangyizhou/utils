#include <iostream>
#include <vector>
#include <string>

class A {
  public:
    A(){}
    A(size_t size): size(size), array((int*) malloc(size)) {
        std::cout 
          << "create Array，memory at: "  
          << array << std::endl;
        
    }
    ~A() {
        free(array);
    }
    A(A &&a) : array(a.array), size(a.size) {
        a.array = nullptr;
        std::cout 
          << "Array moved, memory at: " 
          << array 
          << std::endl;
    }
    A(A &a) : size(a.size) {
        array = (int*) malloc(a.size);
        for(int i = 0;i < a.size;i++)
            array[i] = a.array[i];
        std::cout 
          << "Array copied, memory at: " 
          << array << std::endl;
    }
    size_t size;
    int *array;
};
template<typename T>
void warp(T&& param) {
    if(std::is_rvalue_reference<decltype(param)>::value){
        std::cout<<"param is rvalue reference\n";
    } else {
        std::cout<<"param is lvalue reference\n";
    }
    A y = A(param); // 已经变成了左值
    A z = A(std::forward<T>(param)); // std::forward<T> 转发保持右值特性
}
int main(){
    A a = A(100);
    warp(std::move(a)); // std::move 强制转换成右值
    return 0;   
}

//----------------output----------------
// create Array，memory at: 0x600002e60000 //main函数中，A a = A(100);调用构造函数
// param is rvalue reference //使用了std::move，根据引用折叠规则，param是一个右值引用
// Array copied, memory at: 0x600002e60070 // A y = A(param); 可以看到调用的是拷贝的构造函数
// Array moved, memory at: 0x600002e60000。// A z = A(std::forward<T>(param)); 调用了移动构造函数