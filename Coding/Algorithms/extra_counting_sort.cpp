/*
 * 手撕：计数排序（Counting Sort）
 * 适用于值域不大且非负（或可平移）的整数序列。
 *
 * 解法：统计每个值出现次数，再按值从小到大展开。
 * 时间 O(n+k)，k 为值域宽度；空间 O(k)。
 *
 * 输入：第一行 n，第二行 n 个非负整数（保证 max(a) 合理）。
 * 输出：一行升序序列。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（值域小时替代通用 sort）
// ============================================================================
vector<int> countingSortNonNegative(const vector<int>& a) {
    int mx = 0;
    for (int x : a) mx = max(mx, x);
    vector<int> cnt(mx + 1, 0);
    for (int x : a) ++cnt[x];
    vector<int> out;
    out.reserve(a.size());
    for (int v = 0; v <= mx; ++v)
        for (int c = 0; c < cnt[v]; ++c) out.push_back(v);
    return out;
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
    auto ans = countingSortNonNegative(a);
    for (int i = 0; i < (int)ans.size(); ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
