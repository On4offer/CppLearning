/*
 * 3. 无重复字符的最长子串
 * 给定字符串 s，求不含重复字符的最长子串长度。
 *
 * 解法：滑动窗口 + 数组记录字符上次出现下标（ASCII 假设在 0..127）。
 * 右端扩展；若 s[r] 在窗口内重复，则左端跳到 max(left, last[s[r]]+1)。
 * 时间 O(n)，空间 O(Σ)（字符集大小）。
 *
 * 输入：一行字符串 s（不含空格）。
 * 输出：一个整数，最长无重复子串长度。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 3，可直接粘到 LeetCode 类里）
// ============================================================================
int lengthOfLongestSubstring(string s) {
    vector<int> last(128, -1);
    int ans = 0, L = 0;
    for (int r = 0; r < (int)s.size(); ++r) {
        unsigned char c = s[r];
        if (last[c] >= L) L = last[c] + 1;
        last[c] = r;
        ans = max(ans, r - L + 1);
    }
    return ans;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if (!(cin >> s)) return 0;
    cout << lengthOfLongestSubstring(s) << '\n';
    return 0;
}
