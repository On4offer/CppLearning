/*
 * 手撕：KMP 字符串匹配
 * 在文本串 text 中找模式串 pattern 所有出现起点（0-based）。
 *
 * 输入：两行，分别为 pattern、text（无空格）。
 * 输出：第一行为匹配次数 k；第二行为 k 个起点下标（空格分隔）。
 */
#include <bits/stdc++.h>
using namespace std;

namespace {
vector<int> buildNext(const string& p) {
    int m = (int)p.size();
    vector<int> nxt(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j && p[i] != p[j]) j = nxt[j - 1];
        if (p[i] == p[j]) ++j;
        nxt[i] = j;
    }
    return nxt;
}
}  // namespace

// ============================================================================
// 核心方法
// ============================================================================
vector<int> kmpSearch(const string& haystack, const string& needle) {
    if (needle.empty()) return {};
    vector<int> nxt = buildNext(needle);
    vector<int> res;
    int n = (int)haystack.size(), m = (int)needle.size();
    for (int i = 0, j = 0; i < n; ++i) {
        while (j && haystack[i] != needle[j]) j = nxt[j - 1];
        if (haystack[i] == needle[j]) ++j;
        if (j == m) {
            res.push_back(i - m + 1);
            j = nxt[j - 1];
        }
    }
    return res;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string pat, text;
    if (!(cin >> pat >> text)) return 0;
    vector<int> pos = kmpSearch(text, pat);
    cout << pos.size() << '\n';
    for (size_t i = 0; i < pos.size(); ++i) {
        if (i) cout << ' ';
        cout << pos[i];
    }
    cout << '\n';
    return 0;
}
