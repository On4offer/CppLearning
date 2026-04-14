/*
 * 88. 合并两个有序数组
 * nums1 长度 m+n，前 m 个有效；nums2 长度 n。合并为升序且放在 nums1。
 *
 * 解法：从尾部合并。三指针 i=m-1, j=n-1, k=m+n-1，每次取较大者放到 nums1[k]。
 * 避免覆盖 nums1 未处理的前缀。时间 O(m+n)，空间 O(1)。
 *
 * 输入：
 *   第一行 m n
 *   第二行 m+n 个数：nums1 整体（后 n 个位置为占位 0）
 *   第三行 n 个数：nums2
 * 输出：一行，合并后的 m+n 个数。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 88）
// ============================================================================
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    int i = m - 1, j = n - 1, k = m + n - 1;
    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j])
            nums1[k--] = nums1[i--];
        else
            nums1[k--] = nums2[j--];
    }
    while (j >= 0) nums1[k--] = nums2[j--];
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n;
    if (!(cin >> m >> n)) return 0;
    vector<int> nums1(m + n), nums2(n);
    for (int& x : nums1) cin >> x;
    for (int& x : nums2) cin >> x;
    merge(nums1, m, nums2, n);
    for (int i = 0; i < m + n; ++i) {
        if (i) cout << ' ';
        cout << nums1[i];
    }
    cout << '\n';
    return 0;
}
