/*
 * 46. 全排列
 * 给定互不相同的整数数组，返回所有排列。
 *
 * 解法：回溯 DFS。维护当前路径与 used 标记，每次选一个未用元素下钻，满长度则记录。
 * 时间 O(n·n!)，空间 O(n) 递归栈与标记。
 *
 * 输入：
 *   第一行 n
 *   第二行 n 个互不相同的整数
 * 输出：每个排列一行，数字空格分隔；按字典序可调整（此处按搜索顺序输出）。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 46）
// ============================================================================
vector<vector<int>> permute(vector<int>& nums) {
    int n = (int)nums.size();
    vector<int> path;
    vector<char> used(n, 0);
    vector<vector<int>> res;
    function<void()> dfs = [&]() {
        if ((int)path.size() == n) {
            res.push_back(path);
            return;
        }
        for (int i = 0; i < n; ++i) {
            if (used[i]) continue;
            used[i] = 1;
            path.push_back(nums[i]);
            dfs();
            path.pop_back();
            used[i] = 0;
        }
    };
    dfs();
    return res;
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
    auto res = permute(a);
    for (auto& v : res) {
        for (int i = 0; i < (int)v.size(); ++i) {
            if (i) cout << ' ';
            cout << v[i];
        }
        cout << '\n';
    }
    return 0;
}
