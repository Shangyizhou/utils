#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void selectSort(vector<int>& nums) {
    int len = nums.size();
    // i相当于索引升序数组
    // j用于索引有序数组之后的最小值
    // 所以每次假设i所处的元素为最小值，然后让j往后遍历找寻更小的元素，找到了就交换
    // 因为j=i+1，所以i最多取到len-1
    for (int i = 0; i < len - 1; i++) {
        int min = i;
        for (int j = i + 1; j < len; j++) {
            if (nums[j] < nums[min]) {
                min = j;
            }
        }

        // 用个判断当j找寻到最小值才交换
        if (min != i)
            swap(nums[i], nums[min]);
    }
}

void show(const vector<int>& nums) {
    for (const auto& num : nums) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    vector<int> vec{ 14,33,27,10,35,19,42,44 };
    selectSort(vec);
    show(vec);
    return 0;
}