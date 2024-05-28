#include <iostream>
#include <chrono>
using namespace std::chrono;
using std::cout;
using std::endl;

/**
 * chrono 主要包含三个概念：
 * Durantions：时间段
 * Time points：时间点
 * Clocks：包括system_clock(系统时钟),steady_clock（稳定时钟）;high_resolution_clock（高精度时钟）;
 * 
 * 我们可以使用 Durantions 测量函数执行耗时
 * 使用 Time points 表示一个时间点（时间点其实就是duration，只不过start被设置好了）
 * 使用 clocks 获取当前时间戳，其返回一个 time_point
 * 
*/

long fibonacci(unsigned n)
{
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

void testFunctionTime()
{
    // 获取当前时间戳
    auto start = steady_clock::now();
    fibonacci(42);
    // 获取当前时间戳
    auto end = steady_clock::now();
    // 两次时间戳相减得到 duration
    duration<double> dur = end - start;
    // duration.count() 表示有多少个单位
    // 比如 hours.count() 就是多少个小时
    cout << "fibonacci cost " << dur.count() << "s" << endl;
}

// 基本是核心类了
void testDuration()
{
    // duration的内建类型
    // 第一个参数存储类型，第二个参数存储其与秒的比例
    // 我们所用的 std::chronno 下的 秒，分，时，毫秒，微秒，纳秒 都是 duration 的不同模板实例
    
    // typedef duration<long long, nano> nanoseconds;
    // typedef duration<long long, micro> microseconds;
    // typedef duration<long long, milli> milliseconds;
    // typedef duration<long long> seconds;
    // typedef duration<int, ratio<60> > minutes;
    // typedef duration<int, ratio<3600> > hours;  

    // duration时间单位不同也可以自动转换
    minutes t1(10); // 10min = 600s
    seconds t2(60); // 60s
    seconds t3 = t1 - t2; 
    cout << t3.count() << " seconds" << endl; // 540s

}

/**
 * 类模板 std::chrono::time_point 表示一个时间点。
 * 它的实现方式就好像它存储了一个 Duration 类型的值，该值指示从时钟纪元开始的时间间隔。
*/
void testTimePoint()
{
    // 相较于 duration 是用来记录时间的长度的， time_point 是用来记录一个特定时间点的资料类别。
    // 他一样是一个 template class，需要指定要使用的 clock 与时间单位（duration）

    // clock有两种：system_clock 和 steady_clock
    // system_clock 是直接去抓系统的时间，有可能在使用中会被被修改
    // steady_clock 则是确实地去纪录时间的流逝，所以不会出现时间倒退的状况

    // time_point 两者相减产生一个 duration 结果
    // 可以使用 duration_cast<TYPE> 将结果转换成指定类型
    steady_clock::time_point t1 = steady_clock::now();
    cout << "test the printf sentence time" << endl;
    steady_clock::time_point t2 = steady_clock::now();
    cout << "the sentence cost " << duration_cast<microseconds>(t2 - t1).count() << "us" << endl;

    // 同样可以是用 time_point + duration 进行计算，得到一个新的 time_poing
    steady_clock::time_point t3 = steady_clock::now();
    const steady_clock::time_point t4 = hours(1) + t3;
    
    // STL 的 chrono 并没有定义 time_point 的输出方式，所以我们并不能直接透过 output stream 来输出 time_point
    // 我们可以通过 to_time_t 成员函数转换时间点，但是只有 system_clock 具有这个方法
    system_clock::time_point t5 = system_clock::now();
    system_clock::time_point t6 = hours(1) + t5;
    time_t now_c = system_clock::to_time_t(t6);
    cout << ctime(&now_c) << endl; // Sun Oct 16 15:51:57 2022
}

int main()
{
    // 测试斐波那契函数时间
    // testFunctionTime(); 

    // 测试Duration时间段的用法
    // testDuration(); 

    // 测试时间点的用法
    // testTimePoint();


    return 0;
}