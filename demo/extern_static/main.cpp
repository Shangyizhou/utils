#include <iostream>
extern "C" {
    #include "add.h"
}

using namespace std;
int main() {
    add(2,3);
    return 0;
}