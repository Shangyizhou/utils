#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void show(vector<int>& vec)
{
    cout << "vec.size() = " << vec.size() << " vec.capacity = " << vec.capacity() << endl;
}

// 探索 size 和 capacity 的关系
void test1()
{
    vector<int> vec;
    vec.push_back(1);
    show(vec);
    vec.push_back(2);
    show(vec);
    vec.push_back(3);
    show(vec);
    vec.push_back(4);
    show(vec);
    vec.push_back(5);
    show(vec);

    // vec.size() = 1 vec.capacity = 1
    // vec.size() = 2 vec.capacity = 2
    // vec.size() = 3 vec.capacity = 4
    // vec.size() = 4 vec.capacity = 4
    // vec.size() = 5 vec.capacity = 8
}

// 探索 reserve 和 resize 
void test2()
{
    vector<int> vec;
    vec.reserve(8); // 改变了 capacity 但是 size = 0
    show(vec);
    // 错误，因为 vec2.size() == 0
    // for (int i = 0; i < vec2.size(); i++) {
    for (int i = 0; i < vec.capacity(); i++) {
        vec.push_back(i + 1);
        show(vec);
    }

    // vec.size() = 0 vec.capacity = 8
    // vec.size() = 1 vec.capacity = 8
    // vec.size() = 2 vec.capacity = 8
    // vec.size() = 3 vec.capacity = 8
    // vec.size() = 4 vec.capacity = 8
    // vec.size() = 5 vec.capacity = 8
    // vec.size() = 6 vec.capacity = 8
    // vec.size() = 7 vec.capacity = 8
    // vec.size() = 8 vec.capacity = 8

    vec.resize(5); // 如果小于 capacity，则只改变 size
    show(vec);
    // vec.size() = 5 vec.capacity = 8

    vec.resize(11); // 如果大于 capacity，则只 size 和 capacity 都改变，且为同一个值
    show(vec);
    // vec.size() = 11 vec.capacity = 11

    vec.reserve(5); // reserve的值小于当前的size和capacity，则不会变化
    show(vec);
    // vec.size() = 11 vec.capacity = 11
}

// 释放 vector 的方案 vector<int>().swap()
void test3()
{
    vector<int> vec{1, 2, 3, 4, 5};
    show(vec);
    // vec.size() = 5 vec.capacity = 5


    vec.clear(); // clear可以清空内容，但是还是有这么多内存没有释放
    show(vec);
    // vec.size() = 0 vec.capacity = 5

    // 使用 swap 来释放内存，构建临时对象交换内存，然后临时对象销毁，内存销毁。
    // vector<int>().swap(vec); 
    // 等价于
    {
        vector<int> temp;
        vec.swap(temp); // 现在vec是空的，内容跑到temp里了，而temp出作用域被销毁
        show(vec);      // vec.size() = 0 vec.capacity = 0
        show(temp);     // vec.size() = 0 vec.capacity = 5
    }

    // 标准解决方案
    // template < class T >
    // void ClearVector( vector< T >& vt ) 
    // {
    //     vector< T > vtTemp; 
    //     veTemp.swap( vt );
    // }
}

// 释放 vector 的方案 shrink_to_fit
void test4()
{
    vector<int> vec;
    for (int i = 0; i < 5; i++) {
        vec.push_back(i + 1);
    }
    show(vec); // vec.size() = 5 vec.capacity = 8
    vec.shrink_to_fit(); // capacity 会与 size 适配
    show(vec); // vec.size() = 5 vec.capacity = 5

    // 我们可以先 clear 将 size 置为 0
    // 然后再让 capacity 与 size 适配
    vec.clear();
    show(vec); // vec.size() = 0 vec.capacity = 5
    vec.shrink_to_fit();
    show(vec); // vec.size() = 0 vec.capacity = 0
}

int main()
{
    // test1()
    // test2();
    // test3();
    // test4();
    
    

    return 0;
}