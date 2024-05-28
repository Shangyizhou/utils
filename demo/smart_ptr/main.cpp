#include <iostream>
#include <memory>
using namespace std;

class B; // 前置声明
class A {
public:
    shared_ptr<B> ptr;
    A() {
        cout << "A的构造函数" << endl;
    }
    ~A() {
        cout << "A的析构函数" << endl;
    }
};

class B {
public:
    shared_ptr<A> ptr;
    B() {
        cout << "B的构造函数" << endl;
    }
    ~B() {
        cout << "B的析构函数" << endl;
    }
};

int main()
{
    //倘若我们写出这样的代码，那么理想情况下，每次循环结束释放内存，调用析构函数
    //但是因为循环引用的关系，导致引用计数都为2，循环结束减一，并不会调用类A,B的析构函数释放内存
    //我们的类需要互相有一个指向对面的指针，但是又不想影响彼此的引用计数，那么就使用weak_ptr
    shared_ptr<A> pa(new A());//智能指针pa管理对象(pa.use_count() == 1)  
    shared_ptr<B> pb(new B());//智能指针pb管理对象(pb.use_count() == 1) 
    pa -> ptr = pb;//pa的成员变量指针指向了pb,导致pb的引用计数+1, pb.use_count() == 2
    pb -> ptr = pa;//pb的成员变量指针指向了pa,导致pa的引用计数+1, pa.use_count() == 2
    
    //所以结束的时候,pa的引用技术减去1,但是因为pb内部有成员指向pa,导致pa.use_count() != 0,所以不会析构掉该指针指向对象
    //也因此,pa指向对象不被销毁,其指针仍然指向pb指向对象,使其pb.use_count() != 0,pb也不会被释放


    return 0;
}