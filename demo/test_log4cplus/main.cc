#include "Logger.h"
#include <thread>

void test1()
{
    std::cout << "-----------------test1--------------" << std::endl;
    INFO("start thread 1");
}

void test2()
{
    std::cout << "-----------------test2--------------" << std::endl;
    INFO("start thread 2");
}

int main()
{
    INFO("Main Thread");
    std::thread t1(test1);
    std::thread t2(test2);    
    t1.join();
    t2.join();

    return 0;
}