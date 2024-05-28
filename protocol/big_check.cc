#include <stdint.h>
#include <iostream>
using namespace std;

// 个32位int数的十六进制值为0x01234567（最高有效位——最低有效位）
// 大端法
// 0x100 0x101 0x102 0x103
//   01    23    45    67

// 小端法
// 0x100 0x101 0x102 0x103
//   67    45    23    01
bool bigCheck() {
    union Check {
        char a;
        uint32_t data;
    };
    
    // -

    // 4
    cout << sizeof(Check) << endl;

    Check c;
    c.data = 1;
    
    // 如果相等，说明char a 和 uint32_t data共享的内存空间的字节顺序是小端序（低字节存储在低地址）。在这种情况下，函数返回false
    if (1 == c.a) {
        cout << "small" << endl;
    } else {
        cout << "bag" << endl;
    }

    return true;
}

int main() {
    bigCheck();

    return 0;
}