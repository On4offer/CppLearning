/*
 * 手撕：归并排序（Merge Sort）
 * 分治：将区间二分，递归排序两半，再线性合并两个有序段。
 *
 * 解法：merge(l,mid,r) 双指针归并到临时数组，再拷回原数组。
 * 时间 O(n log n)，空间 O(n)（辅助数组）。
 *
 * 输入：第一行 n，第二行 n 个整数。
 * 输出：一行升序序列。
 */
#include <bits/stdc++.h>
using namespace std;

namespace {
void mergeSort(vector<int>& a, vector<int>& tmp, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSort(a, tmp, l, mid);
    mergeSort(a, tmp, mid + 1, r);
    int i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int p = l; p <= r; ++p) a[p] = tmp[p];
}
}  // namespace

// ============================================================================
// 核心方法（可对照 LeetCode 912 sortArray）
// ============================================================================
vector<int> sortArray(vector<int>& nums) {
    vector<int> tmp(nums.size());
    if (!nums.empty()) mergeSort(nums, tmp, 0, (int)nums.size() - 1);
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
