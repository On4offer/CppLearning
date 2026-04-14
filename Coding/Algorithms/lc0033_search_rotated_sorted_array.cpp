/*
 * 33. 搜索旋转排序数组
 * 升序数组在某处旋转后，无重复元素，判断 target 是否存在，存在则输出下标，否则 -1。
 *
 * 解法：二分。中点与端点比较判断哪一半有序；在有序半段内判断 target 是否落在区间内以收缩边界。
 * 时间 O(log n)，空间 O(1)。
 *
 * 输入：
 *   第一行 n、target
 *   第二行 n 个互不相同的整数（旋转后的数组）
 * 输出：下标（0-based）或 -1。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 33）
// ============================================================================
int search(vector<int>& nums, int target) {
    int n = (int)nums.size();
    if (n == 0) return -1;
    int L = 0, R = n - 1;
    while (L <= R) {
        int M = L + (R - L) / 2;
        if (nums[M] == target) return M;
        if (nums[L] <= nums[M]) {
            if (nums[L] <= target && target < nums[M])
                R = M - 1;
            else
                L = M + 1;
        } else {
            if (nums[M] < target && target <= nums[R])
                L = M + 1;
            else
                R = M - 1;
        }
    }
    return -1;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, target;
    if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    cout << search(a, target) << '\n';
    return 0;
}
