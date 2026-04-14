/*
 * 手撕：下一个排列（Next Permutation）
 * 存在下一个排列则变换并返回 true，否则返回 false。
 *
 * 输入：第一行 n，第二行 n 个互不相同整数。
 * 输出：下一排列一行；若已是最后一个则输出一行 -1。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（可对照 LeetCode 31；本题为 bool 版便于 ACM）
// ============================================================================
bool nextPermutation(vector<int>& nums) {
    int n = (int)nums.size();
    int i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (nums[j] <= nums[i]) --j;
    swap(nums[i], nums[j]);
    reverse(nums.begin() + i + 1, nums.end());
    return true;
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
    if (!nextPermutation(a)) {
        cout << -1 << '\n';
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << '\n';
    return 0;
}
