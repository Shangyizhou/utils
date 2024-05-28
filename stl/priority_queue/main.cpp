#include <iostream>
#include <utility>
#include <queue>

void testPriorityQueue()
{
    // 小顶堆：升序队列
    std::priority_queue<int, std::vector<int>, std::greater<int>> small_pq;
    // 大顶堆：降序队列
    std::priority_queue<int, std::vector<int>, std::less<int>> big_pq;

    std::vector<int> vec = {5, -10, 6, 3, 1, 7, 2};
    for (const auto& v : vec) {
        small_pq.push(v);
        big_pq.push(v);
    }

    std::cout << "小顶堆输出" << std::endl;
    while (!small_pq.empty()) {
        std::cout << small_pq.top() << " ";
        small_pq.pop();
    }
    std::cout << std::endl;

    std::cout << "大顶堆输出" << std::endl;
    while (!big_pq.empty()) {
        std::cout << big_pq.top() << " ";
        big_pq.pop();
    }
    std::cout << std::endl;
}

// 自定义优先级
struct compare {
    bool operator()(const std::pair<char, int> a, const std::pair<char, int> b) {
        return a.second < b.second;
    }
};

void testFunc()
{
    std::priority_queue<std::pair<char, int>, std::vector<std::pair<char, int>>, compare> pq;
    pq.push(std::make_pair<char, int>('a', 3));
    pq.push(std::make_pair<char, int>('b', 1));
    pq.push(std::make_pair<char, int>('c', 2));

    while (!pq.empty()) {
        auto temp = pq.top();
        std::cout << temp.first << " " << temp.second << std::endl;
        pq.pop();
    }
}

void testLambda()
{
    // 使用lambda表达式
    auto compare_lambda = [](const std::pair<char, int>& a, const std::pair<char, int>& b){
        return a.second < b.second;
    };
    std::priority_queue<std::pair<char, int>, std::vector<std::pair<char, int>>, decltype(compare_lambda)> pq(compare_lambda);
    pq.push(std::make_pair<char, int>('a', 3));
    pq.push(std::make_pair<char, int>('b', 1));
    pq.push(std::make_pair<char, int>('c', 2));

    while (!pq.empty()) {
        auto temp = pq.top();
        std::cout << temp.first << " " << temp.second << std::endl;
        pq.pop();
    }
}


int main()
{
    // testFunc();
    testLambda();

    return 0;
}