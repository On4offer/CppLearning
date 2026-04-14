/*
 * 215. 数组中的第 K 个最大元素（解法二）
 *
 * 解法：小根堆维护 k 个元素。遍历数组，堆大小超过 k 则弹出最小值；
 * 结束后堆顶即第 k 大。时间 O(n log k)，空间 O(k)。
 *
 * 输入/输出：同 lc0215_kth_largest_element.cpp
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 215，堆）
// ============================================================================
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int x : nums) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();
    }
    return pq.top();
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
