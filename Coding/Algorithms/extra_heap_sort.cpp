/*
 * 手撕：堆排序（Heap Sort）
 * 原地建大顶堆，每次把堆顶（最大值）换到末尾并缩小堆。
 *
 * 解法：siftDown 维护 [0..i] 大根堆性质；从最后一个非叶结点向上建堆；
 * 再 n-1 次弹出堆顶。时间 O(n log n)，空间 O(1)。
 *
 * 输入：第一行 n，第二行 n 个整数。
 * 输出：一行升序序列。
 */
#include <bits/stdc++.h>
using namespace std;

namespace {
void siftDown(vector<int>& a, int n, int i) {
    while (true) {
        int L = i * 2 + 1, R = i * 2 + 2, m = i;
        if (L < n && a[L] > a[m]) m = L;
        if (R < n && a[R] > a[m]) m = R;
        if (m == i) break;
        swap(a[i], a[m]);
        i = m;
    }
}
}  // namespace

// ============================================================================
// 核心方法（可对照 LeetCode 912 sortArray）
// ============================================================================
vector<int> sortArray(vector<int>& nums) {
    int n = (int)nums.size();
    for (int i = n / 2 - 1; i >= 0; --i) siftDown(nums, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(nums[0], nums[i]);
        siftDown(nums, i, 0);
    }
    return nums;
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
    sortArray(a);
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << '\n';
    return 0;
}
