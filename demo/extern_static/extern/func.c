#include <stdio.h>

//默认情况下,函数自带extern效果(外部函数)
extern void func() {
	printf("调用了func函数\n");
}
