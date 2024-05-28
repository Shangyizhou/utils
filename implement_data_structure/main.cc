#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

class String
{
public:
	String(const char *str = NULL);	//通用构造函数，这包含了默认构造函数
	String(const String &str);		//拷贝构造函数
	~String();						//析构函数

	String operator+(const String &str) const;	//重载+
	String& operator=(const String &str);		//重载=
	String& operator+=(const String &str);		//重载+=
	bool operator==(const String &str) const;	//重载==
	char& operator[](int n) const;				//重载[]

	size_t size() const;		//获取长度
	const char* c_str() const;	//获取C字符串

	friend istream& operator>>(istream &is, String &str);//输入
	friend ostream& operator<<(ostream &os, const String &str);//输出

private:
	char *data;		//字符串
	size_t length;	//长度
};

String::String(const char *str)//通用构造函数
{
	if (!str)
	{
		length = 0;
		data = new char[1];
		*data = '\0';
	}
	else
	{
		length = strlen(str);
		data = new char[length + 1];
		strcpy(data, str);
	}
}

String::String(const String &str)//拷贝构造函数
{
	length = str.size();
	data = new char[length + 1];
	strcpy(data, str.c_str());
}

String::~String()//析构函数
{
	delete []data;
	length = 0;
}

String String::operator+(const String &str) const//重载+
{
	String newString;
	newString.length = length + str.size();
	newString.data = new char[newString.length + 1];
	strcpy(newString.data, data);
	strcat(newString.data, str.data);
	return newString;
}

String& String::operator=(const String &str)//重载=
{
	if (this == &str) {
        return *this;
    }

	delete []data;
	length = str.length;
	data = new char[length + 1];
	strcpy(data, str.c_str());
	return *this;
}

String& String::operator+=(const String &str)//重载+=
{
	length += str.length;
	char *newData = new char[length + 1];
	strcpy(newData, data);
	strcat(newData, str.data);
	delete []data;
	data = newData;
	return *this;
}

inline bool String::operator==(const String &str) const//重载==
{
	if (length != str.length)	return false;
	return strcmp(data, str.data) ? false : true;
}

inline char& String::operator[](int n) const//重载[]
{
	if (n >= length) return data[length-1]; //错误处理
	else return data[n];
}

istream& operator>>(istream &is, String &str)//输入
{
	char tem[1000];  //简单的申请一块内存
	is >> tem;
	str.length = strlen(tem);
	str.data = new char[str.length + 1];
	strcpy(str.data, tem);
	return is;
}

ostream& operator<<(ostream &os, const String &str)//输出
{
	os << str.data;
	return os;
}

inline size_t String::size() const
{
    return length;
}

inline const char* String::c_str() const//获取C字符串
{
	return data;
}

int main()
{
	String s;	
	cin >> s;	
	cout << s << ": " << s.size() << endl;

	char a[] = "Hello", b[] = "World!";
	String s1(a), s2(b);	
	cout << s1 << " + " << s2 << " = " << (s1 + s2) << endl;

	String s3 = s1 + s2;
	if (s1 == s3)	cout << "First: s1 == s3" << endl;
	s1 += s2;
	if (s1 == s3)	cout << "Second: s1 == s3" << endl;

	/*程序输入输出为：
	123456789
	123456789: 9
	Hello + World! = HelloWorld!
	Second: s1 == s3
	Press any key to continue . . .
	*/
}
// bubble sort


