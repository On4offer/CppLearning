/*
 * 手撕：插入排序（Insertion Sort）
 * 小规模数据或近乎有序时常用；稳定排序。
 *
 * 解法：从左到右，把 a[i] 向前插入到左侧已排序段中的正确位置。
 * 最坏 O(n^2)，最好 O(n)，空间 O(1)。
 *
 * 输入：第一行 n，第二行 n 个整数。
 * 输出：一行升序序列。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（可对照 LeetCode 912 sortArray）
// ============================================================================
vector<int> sortArray(vector<int>& nums) {
    int n = (int)nums.size();
    for (int i = 1; i < n; ++i) {
        int key = nums[i], j = i - 1;
        while (j >= 0 && nums[j] > key) {
            nums[j + 1] = nums[j];
            --j;
        }
        nums[j + 1] = key;
    }
    return nums;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    sortArray(a);
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << '\n';
    return 0;
}
