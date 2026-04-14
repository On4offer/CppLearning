/*
 * 300. 最长上升子序列（LIS）
 * 求严格递增子序列的最大长度。
 *
 * 解法：耐心排序 + 二分。维护数组 tail；对每个 x，lower_bound 找第一个 >= x 的位置替换。
 * 时间 O(n log n)，空间 O(n)。
 *
 * 输入：
 *   第一行 n
 *   第二行 n 个整数
 * 输出：LIS 长度。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 300，二分 + tail）
// ============================================================================
int lengthOfLIS(vector<int>& nums) {
    vector<int> tail;
    for (int x : nums) {
        auto it = lower_bound(tail.begin(), tail.end(), x);
        if (it == tail.end())
            tail.push_back(x);
        else
            *it = x;
    }
    return (int)tail.size();
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
    cout << lengthOfLIS(a) << '\n';
    return 0;
}
