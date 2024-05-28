#include <iostream>
#include <algorithm>
using namespace std;
// [start len] 为需要排列的无序列区域
void max_heapify(int arr[], int start, int end) {
    //建立父节点指标和子节点指标
    int dad = start;
    int son = dad * 2 + 1;
    while (son <= end) { //若子节点指标在范围内才做比较
        if (son + 1 <= end && arr[son] < arr[son + 1]) //先比较两个子节点大小，选择最大的
            son++;
        if (arr[dad] > arr[son]) //如果父节点大于子节点代表调整完毕，直接跳出函数
            return;
        else { //否则交换父子内容再继续子节点和孙节点比较
            swap(arr[dad], arr[son]);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

void heap_sort(int arr[], int len) {
    //初始化，i从最后一个父节点开始调整
    //第一次建立堆，可以将最大值放到最顶上，但是不能保证所有细节多是对的
    //因此，之后重新插入排序，就可以在第一步确定第一个值为有序区间，因为它是最大的
    for (int i = len / 2 - 1; i >= 0; i--)
        max_heapify(arr, i, len - 1);
    //先将第一个元素和已经排好的元素前一位做交换，再从新调整(刚调整的元素之前的元素)，直到排序完毕
    //与最后一个元素交换，所以最后一个元素处于有序，不参加重排
    for (int i = len - 1; i > 0; i--) {
        swap(arr[0], arr[i]); 
        max_heapify(arr, 0, i - 1);//无序区间[0 i-1]
    }
}

int main() {
    // int arr[] = { 3, 5, 3, 0, 8, 6, 1, 5, 8, 6, 2, 4, 9, 4, 7, 0, 1, 8, 9, 7, 3, 1, 2, 5, 9, 7, 4, 0, 2, 6 };
    int arr[] = { 3, 5, 3, 0, 8, 6 };
    int len = (int) sizeof(arr) / sizeof(*arr);
    heap_sort(arr, len);
    for (int i = 0; i < len; i++)
        cout << arr[i] << ' ';
    cout << endl;
    return 0;
}