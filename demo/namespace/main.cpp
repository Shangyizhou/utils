#include <iostream>
#include "compare.h"
using compare::max;
using compare::min;

using std::cout;
using std::endl;

/*
忘记编译compare.cpp
main.cpp:(.text+0x90): undefined reference to `compare::min(double const*, int)'
main.cpp:(.text+0xd4): undefined reference to `compare::max(double const*, int)'
*/
int main()
{
    double data[] = {1.2,3.3,5.4,2.6,4.7,6.8};
	const int dataSize = sizeof(data) / sizeof(data[0]);
	cout << "Min double is " << min(data, dataSize) << endl;
	cout << "Max double is " << max(data, dataSize) << endl;

    return 0;
}