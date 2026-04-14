/*
 * 补充题 4. 手撕快速排序
 * 对数组原地升序排序（面试常考 Lomuto / Hoare 划分）。
 *
 * 解法：Lomuto 划分——选末元素为 pivot，[l..i) 为 < pivot，扫描 j 交换小元素到左侧；
 * 递归排序左右两段。期望 O(n log n)，最坏 O(n^2)。
 *
 * 输入：
 *   第一行 n
 *   第二行 n 个整数
 * 输出：一行，升序数组。
 */
#include <bits/stdc++.h>
using namespace std;

namespace {
int partition(vector<int>& a, int l, int r) {
    int pivot = a[r];
    int i = l;
    for (int j = l; j < r; ++j) {
        if (a[j] < pivot) swap(a[i++], a[j]);
    }
    swap(a[i], a[r]);
    return i;
}

void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int p = partition(a, l, r);
    quickSort(a, l, p - 1);
    quickSort(a, p + 1, r);
}
}  // namespace

// ============================================================================
// 核心方法（可对照 LeetCode 912 sortArray）
// ============================================================================
vector<int> sortArray(vector<int>& nums) {
    if (!nums.empty()) quickSort(nums, 0, (int)nums.size() - 1);
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
