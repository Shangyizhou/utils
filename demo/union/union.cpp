#include <iostream>
using namespace std;

union Foo
{
    int a;
    double b;
};

int main()
{
    Foo foo;
    foo.a = 10;
    cout << foo.a << endl; // 10

    foo.b = 20.0;
    cout << foo.a << " " << foo.b << endl; // 0 20

    return 0;
}