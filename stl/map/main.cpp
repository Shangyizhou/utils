#include <iostream>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
using namespace std;

void testMap()
{
    map<string, size_t> word_count;
    string word;
    while (cin >> word) {
        ++word_count[word];
    }
    for (const auto& w : word_count) {
        cout << w.first << " occuers " << w.second << ((w.second > 1) ? "times" : "time") << endl;
    }    
}

// 统计不在集合中的单词数量
void testSet()
{
    map<string, size_t> world_count;
    set<string> exclude = {"The", "Buf", "And", "Or"};
    string world;
    while (cin >> world) {
        // 如果输入词汇不在exclude内，则词汇加 1
        if (exclude.find(world) == exclude.end()) {
            ++world_count[world];
        }
    }
}

void testCreate()
{
    // 关联容器空初始化
    map<string, size_t> word_count;

    // 关联容器列表初始化
    set<string> exclude = {"first", "second", "third"};
    map<string, string> authors = {
        {"zhang san", "man"},
        {"li si", "man"},
        {"xiao zhang", "woman"}
    };

    // first second third
    for (const auto& it : exclude) {
        cout << it << " ";
    }
    cout << endl;

    // li si man
    // xiao zhang woman
    // zhang san man
    for (const auto& it : authors) {
        cout << it.first << " " << it.second << endl;
    }   
}

void testPair()
{
    pair<string, int> words_count;
    pair<string, string> authors = {"zhang san", "book"};
    cout << authors.first << " " << authors.second << endl;

    unordered_map<int, int> element_index = {make_pair<int, int>(1, 0)}; 
    const auto it = element_index.begin();
    cout << it->first << " " << it->second << endl; // 1 0
}

void testInesrt()
{
    // 有重复key，所以使用multiset
    unordered_multiset<string> sentence = {"hello", "world", "hello", "world"};
    map<string, int> word_count;
    for (const auto& element : sentence) {
        word_count[element]++;
        // word_count.insert(make_pair(element, 1));
    }
    for (const auto& it : word_count) {
        cout << it.first << " " << it.second << endl;
    }
}

//统计没有出现在exclude中字符串的次数
int main(int argc, char* argv[])
{
    testInesrt();

    return 0;
}