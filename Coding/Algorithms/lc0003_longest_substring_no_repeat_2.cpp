/*
 * 3. 无重复字符的最长子串（解法二）
 *
 * 解法：滑动窗口 + unordered_set 维护当前窗口内字符集合。
 * 右端加入；若重复则从左端不断 erase 直到可加入。
 * 时间 O(n) 均摊，空间 O(min(n, Σ))。
 *
 * 输入：一行字符串 s（不含空格）。
 * 输出：最长无重复子串长度。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 3）
// ============================================================================
int lengthOfLongestSubstring(string s) {
    unordered_set<char> win;
    int ans = 0, L = 0;
    for (int r = 0; r < (int)s.size(); ++r) {
        while (win.count(s[r])) win.erase(s[L++]);
        win.insert(s[r]);
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
