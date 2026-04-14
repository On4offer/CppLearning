/*
 * 5. 最长回文子串
 * 求字符串 s 中最长的连续回文子串（若有多个，输出第一次按中心扩展得到的最长之一）。
 *
 * 解法：中心扩展。枚举中心为奇数（单字符）与偶数（两字符之间），向两侧扩展。
 * 时间 O(n^2)，空间 O(1)。
 *
 * 输入：一行字符串 s。
 * 输出：最长回文子串。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 5）
// ============================================================================
static string expand(const string& s, int L, int R) {
    while (L >= 0 && R < (int)s.size() && s[L] == s[R]) {
        --L;
        ++R;
    }
    return s.substr(L + 1, R - L - 1);
}

string longestPalindrome(string s) {
    string ans;
    for (int i = 0; i < (int)s.size(); ++i) {
        string a = expand(s, i, i);
        string b = expand(s, i, i + 1);
        if (a.size() > ans.size()) ans = a;
        if (b.size() > ans.size()) ans = b;
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
    cout << longestPalindrome(s) << '\n';
    return 0;
}
