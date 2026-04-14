/*
 * 手撕：二分查找（有序数组中是否存在 target）
 * 若存在输出最小下标，否则 -1。
 *
 * 输入：
 *   第一行 n q（数组长度、查询次数）
 *   第二行 n 个升序整数
 *   接下来 q 行，每行一个 target
 * 输出：每行一个答案：存在则最小下标（0-based），否则 -1。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（可对照 lower_bound 行为）
// ============================================================================
int searchFirst(const vector<int>& nums, int target) {
    int L = 0, R = (int)nums.size() - 1, ans = -1;
    while (L <= R) {
        int M = L + (R - L) / 2;
        if (nums[M] == target) {
            ans = M;
            R = M - 1;
        } else if (nums[M] < target)
            L = M + 1;
        else
            R = M - 1;
    }
    return ans;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    while (q--) {
        int t;
        cin >> t;
        cout << searchFirst(a, t) << '\n';
    }
    return 0;
}
