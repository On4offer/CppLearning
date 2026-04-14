/*
 * 手撕：最大子数组和（Kadane）
 * 给定整数数组（可含负数），求连续子数组的最大和。
 *
 * 输入：第一行 n，第二行 n 个整数。
 * 输出：一个整数，最大子段和。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（可对照 LeetCode 53 maxSubArray）
// ============================================================================
int maxSubArray(vector<int>& nums) {
    long long cur = nums[0], ans = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        cur = max<long long>(nums[i], cur + nums[i]);
        ans = max(ans, cur);
    }
    return (int)ans;
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
    for (auto& x : a) cin >> x;
    cout << maxSubArray(a) << '\n';
    return 0;
}
