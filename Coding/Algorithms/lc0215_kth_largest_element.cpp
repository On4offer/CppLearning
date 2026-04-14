/*
 * 215. 数组中的第 K 个最大元素
 * 返回升序排序后倒数第 k 个（即第 k 大，1-indexed）。
 *
 * 解法：快速选择。将 >= pivot 的放左侧，使 pivot 下标为 p；
 * 若 p==k-1 则返回；否则只在左或右半区间继续。
 * 期望时间 O(n)，空间 O(1) 加递归栈。
 *
 * 输入：
 *   第一行 n、k（k 从 1 开始，且 1<=k<=n）
 *   第二行 n 个整数
 * 输出：第 k 大的整数。
 */
#include <bits/stdc++.h>
using namespace std;

namespace {
int partitionDesc(vector<int>& a, int L, int R) {
    int pivot = a[R];
    int i = L;
    for (int j = L; j < R; ++j) {
        if (a[j] >= pivot) swap(a[i++], a[j]);
    }
    swap(a[i], a[R]);
    return i;
}
}  // namespace

// ============================================================================
// 核心方法（题号 215，快速选择）
// ============================================================================
int findKthLargest(vector<int>& nums, int k) {
    int L = 0, R = (int)nums.size() - 1;
    while (true) {
        int p = partitionDesc(nums, L, R);
        if (p == k - 1) return nums[p];
        if (p < k - 1)
            L = p + 1;
        else
            R = p - 1;
    }
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    cout << findKthLargest(a, k) << '\n';
    return 0;
}
