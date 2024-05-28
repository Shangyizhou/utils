#include <iostream>
#include <string>
using namespace std;

// 构造函数
void construct()
{
    string s1; // 空字符串
    string s2("hello world"); // hello world
    string s3(5, 'a'); // aaaaa
    string s4(s3, 2); // aaa：从s3字符串的索引2位置开始构造
    string s5(s2.begin(), s2.end()); // hello world
    string s6(s2.begin(), s2.begin() + 3); // hel：STL形式是左闭右开[)
    
    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;
    cout << s5 << endl;
    cout << s6 << endl;
}

void insert()
{
    // +=
    string s1("hello ");
    s1 += "world";
    cout << s1 << endl; // hello world

    // append
    string s2("hello ");
    s2.append("world", 3); // 只在末尾插入前3个字符
    cout << s2 << endl; // hello wor

    string s3("hello ");
    s3.append("world", 1, 5); // 只将 "world" 的 [1, 5) 位置插入
    cout << s3 << endl; // hello orld

    // push_back
    string s4("hello ");
    s4.push_back('w');
    s4.push_back('o');
    s4.push_back('r');
    s4.push_back('l');
    s4.push_back('d');
    cout << s4 << endl; // hello world

    // insert
    string s5("hello ");
    s5.insert(6, "world"); // 从索引6开始插入（索引位置会被覆盖，之前的索引长度最多为5，因此从索引6开始）
    cout << s5 << endl; // hello world
    
    string s6("hello ");
    s6.insert(string::size_type(6), 2, '!'); // 在索引5处向后插入两个字符串 '!'
    cout << s6 << endl; // hello !!

}

void erase()
{
    string s1("hello world");
    s1.erase(0, 1); // 删除[0, 1)的字符
    cout << s1 << endl; // ello world

    string s2("hello world");
    s2.erase(1); // 从1开始删除到末尾
    cout << s2 << endl; // h
}

// 查询字符在字符串位置
// 查询失败返回string::npos
// string::npos是字符串可储存的最大字符数，通常是无符号int或无符号long的最大取值。
// 一般有三个参数，需要查询的字符串，从哪里开始查询，查询字符串的前几个字符
void find()
{
    string s1("hello world");

    // size_type find(const char ch, size_type pos = 0) const
    // 从字符串的pos位置开始，查找字符ch。如果找到，则返回该子字符串首次出现的位置；否则，返回string::npos
    int index = s1.find('o');
    cout << index << endl; 

    // 指定从索引8开始找，找不到返回-1
    index = s1.find('o', 8); 
    cout << index << endl; 

    // size_type find(const string & str, size_type pos = 0) const
    // 从字符串的pos位置开始，查找子字符串str。如果找到，则返回该子字符串首次出现时其首字符的索引；否则，返回string::npos
    index = s1.find("llo");
    cout << index << endl; // 2

    // 规定从索引3开始找，所以找不到 "llo"
    index = s1.find("llo", 3);
    cout << index << endl; // -1

    // size_type find(const char * s, size_type pos = 0, size_type n) const	
    // 从字符串的pos位置开始，查找s的前n个字符组成的子字符串。如果找到，则返回该子字符串首次出现时其首字符的索引；否则，返回string::npos
    index = s1.find("lloo", 0);
    cout << index << endl; // -1

    // "lloo"找不到，这一次指定只找前三个字符
    index = s1.find("lloo", 0, 3);
    cout << index << endl; // 2

    // find_first_of()方法在字符串中查找参数中任何一个字符首次出现的位置
    index = s1.find_first_of("abcdefg"); // 1：第一个出现在 "abcedfg" 中的字符是 'e'
    cout << "find_first_of " << index << endl;

    // find_last_of()方法在字符串中查找参数中任何一个字符最后一次出现的位置。
    index = s1.find_last_of('o'); // 8
    cout << index << endl;

    // find_first_not_of()方法在字符串中查找第一个不包含在参数中的字符
    index = s1.find_first_not_of("helo "); // 6：s1第一个没出现在"helo "的字符是'w'
    cout << index << endl;

    // find_last_not_of()方法在字符串中查找最后一个不包含在参数中的字符。
    index = s1.find_last_not_of("helo "); // 6：s1最后一个没出现在"helo "的字符是'd'
    cout << index << endl;
}

// 切割字符串
void spilt()
{
    string s1("abcdefg");
    string s2 = s1.substr(0, 3); // abc：[0, 3) 切割了索引[0 1 2]的字符
    string s3 = s1.substr(3); // defg：从指定位置切割到尾部

    cout << s2 << endl;
    cout << s3 << endl;
}

// 字符串与数字转换
void convert()
{
    string s1("12345"); 
    int n1 = stoi(s1);  // "12345" -> 12345
    string s2(to_string(n1)); // 12345 -> "12345"

    string s3("3.14");
    double n2 = stod(s3); // "3.14" -> 3.14
    string s4(to_string(n2)); // 3.140000(double 8 bit)

    cout << s1 << endl;
    cout << n1 << endl;
    cout << s2 << endl;
    cout << n2 << endl;
    cout << s4 << endl;
}

int main()
{
    // construct();
    // insert();
    // erase();
    // find();
    // spilt();
    // convert();

    return 0;
}
