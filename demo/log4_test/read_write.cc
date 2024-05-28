#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    // fwead/fwrite 内部调用了 read/write 函数
    // read/write是系统调用，要自己分配缓存
    // fread/fwrite是标准输入/输出函数，不需要自己分配缓存

    return 0;
}