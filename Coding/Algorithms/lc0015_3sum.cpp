/*
 * 15. 三数之和
 * 找出所有和为 0 的三元组，且三元组不重复（下标不同，值可重复但组合去重）。
 *
 * 解法：先排序；固定第一个数 nums[i]，在右侧双指针 L,R 找两数之和为 -nums[i]。
 * 跳过相邻重复元素避免重复三元组。时间 O(n^2)，空间 O(1) 不计输出。
 *
 * 输入：
 *   第一行 n
 *   第二行 n 个整数
 * 输出：每行三个整数，按字典序输出所有不重复三元组；若无则输出一行 0。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 15）
// ============================================================================
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    int n = (int)nums.size();
    vector<vector<int>> ans;
    for (int i = 0; i < n; ++i) {
        if (i && nums[i] == nums[i - 1]) continue;
        int L = i + 1, R = n - 1;
        while (L < R) {
            long long s = (long long)nums[i] + nums[L] + nums[R];
            if (s < 0)
                ++L;
            else if (s > 0)
                --R;
            else {
                ans.push_back({nums[i], nums[L], nums[R]});
                int lv = nums[L], rv = nums[R];
                while (L < R && nums[L] == lv) ++L;
                while (L < R && nums[R] == rv) --R;
            }
        }
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
    auto ans = threeSum(a);
    if (ans.empty()) {
        cout << "0\n";
        return 0;
    }
    for (auto& t : ans) cout << t[0] << ' ' << t[1] << ' ' << t[2] << '\n';
    return 0;
}
