/*
 * 300. 最长上升子序列（解法二）
 *
 * 解法：DP。dp[i] 表示以 nums[i] 结尾的最长上升子序列长度。
 * 时间 O(n^2)，空间 O(n)。
 *
 * 输入/输出：同 lc0300_longest_increasing_subsequence.cpp
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 300，O(n^2) DP）
// ============================================================================
int lengthOfLIS(vector<int>& nums) {
    int n = (int)nums.size();
    if (n == 0) return 0;
    vector<int> dp(n, 1);
    int ans = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) dp[i] = max(dp[i], dp[j] + 1);
        }
        ans = max(ans, dp[i]);
    }
    return ans;
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
