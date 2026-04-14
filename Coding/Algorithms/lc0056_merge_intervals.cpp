/*
 * 56. 合并区间
 * 若干闭区间 [l,r]，合并所有重叠区间。
 *
 * 解法：按左端点排序，扫描维护当前合并段 [curL,curR]；
 * 若下一区间左端 <= curR 则扩展右端，否则推入答案并开启新区间。
 * 时间 O(n log n)，空间 O(1) 不计答案。
 *
 * 输入：
 *   第一行 n（区间个数）
 *   接下来 n 行，每行 l r
 * 输出：合并后的区间，每行 l r，按左端排序。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 56）
// ============================================================================
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> ans;
    int L = intervals[0][0], R = intervals[0][1];
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] <= R)
            R = max(R, intervals[i][1]);
        else {
            ans.push_back({L, R});
            L = intervals[i][0];
            R = intervals[i][1];
        }
    }
    ans.push_back({L, R});
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
    vector<vector<int>> intervals(n, vector<int>(2));
    for (int i = 0; i < n; ++i) cin >> intervals[i][0] >> intervals[i][1];
    auto ans = merge(intervals);
    for (auto& p : ans) cout << p[0] << ' ' << p[1] << '\n';
    return 0;
}
