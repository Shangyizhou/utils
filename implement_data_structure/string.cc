#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

class String {
private:
    char* data_;
    int len_;

public:
    String(const char* str = nullptr);
    String(const String& str);
    ~String();

    
    String& operator+=(const String& str);
    String& operator=(const String& str);
    bool operator==(const String& str) const;
    char operator[](int n) const;

    const int len() const {
        return len_;
    }

    friend String operator+(const String& a, const String& b);

    // 重载输入输出运算符
    friend ostream& operator<<(ostream& os, const String& str);
    friend istream& operator>>(istream& os, String& str);
};

String::String(const char* str) {
    if (!str) {
        data_ = new char[1];
        data_[0] = '/0';
        len_ = 1;
    } else {
        int len = strlen(str);
        data_ = new char[len + 1];
        strcpy(data_, str);
    }
}

String::String(const String& str) {
    data_ = new char[str.len() + 1];
    strcpy(data_, str.data_);
}

String::~String()//析构函数
{
	delete []data_;
	len_ = 0;
}

String operator+(const String& a, const String& b) {
    int new_len = a.len_ + b.len_;
    String new_str = new char[new_len + 1];
    strcpy(new_str.data_, a.data_);
    strcpy(new_str.data_ + a.len_, b.data_);
    return new_str;
}

ostream& operator<<(ostream& os, const String& str) {
    os << str.data_;
}

istream& operator>>(istream &is, String &str) {
	char tem[1000];  //简单的申请一块内存
	is >> tem;
	str.len_ = strlen(tem);
	str.data_ = new char[str.len_ + 1];
	strcpy(str.data_, tem);
	return is;
}

int main() {
    String str("hello world!");
    String str2(str);
    String str3;

    cout << str << " " << str2 << endl;
    cout << str3.len() << endl;

    cin >> str3;
    cout << str3 << endl;
    cout << str3.len() << endl;
    
    return 0;
}