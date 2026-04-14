/*
 * 5. 最长回文子串（解法二）
 *
 * 解法：区间 DP。dp[i][j] 表示 s[i..j] 是否回文；转移：s[i]==s[j] 且 (j-i<3 或 dp[i+1][j-1])。
 * 记录最大长度起点。时间 O(n^2)，空间 O(n^2)。
 *
 * 输入：一行字符串 s。
 * 输出：最长回文子串。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 5）
// ============================================================================
string longestPalindrome(string s) {
    int n = (int)s.size();
    if (n == 0) return "";
    vector<vector<char>> dp(n, vector<char>(n, 0));
    int bestL = 0, bestLen = 1;
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i; j < n; ++j) {
            if (s[i] == s[j] && (j - i < 2 || (i + 1 < n && j > 0 && dp[i + 1][j - 1]))) {
                dp[i][j] = 1;
                if (j - i + 1 > bestLen) {
                    bestLen = j - i + 1;
                    bestL = i;
                }
            }
        }
    }
    return s.substr(bestL, bestLen);
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
