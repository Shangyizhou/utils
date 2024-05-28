#include <stdio.h>
#include <unistd.h>
#if 0
它可以在进程中设置一个定时器，当定时器指定的时间到时，
它向进程发送SIGALRM信号。可以设置忽略或者不捕获此信号，
如果采用默认方式其动作是终止调用该alarm函数的进程。
#endif
int main()
{
    int i;
    alarm(1); // 1s后发出信号，会终止掉循环

    for (i = 0; ; i++) {
        printf("%d\n", i);
    }

    // 可以使用time命令查看运行时间
    return 0;
}