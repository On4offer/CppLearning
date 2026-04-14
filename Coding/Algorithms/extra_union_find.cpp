/*
 * 手撕：并查集（Union-Find / Disjoint Set）
 * 维护若干集合的合并与查询是否同集合；路径压缩 + 按秩合并保证近似 O(α(n))。
 *
 * 解法：parent[x] 指向父结点；Find 递归/迭代并压缩路径；Union 将两树根按秩合并。
 *
 * 输入：
 *   第一行 n q（元素 0..n-1，操作数）
 *   接下来 q 行：F x y 查询 x y 是否同集（输出 YES/NO）；U x y 合并
 * 输出：每个 F 对应一行。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 手撕核心（模板题常用 DSU 类）
// ============================================================================
struct DSU {
    vector<int> p, r;
    explicit DSU(int n) : p(n), r(n, 0) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) ++r[a];
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    DSU dsu(n);
    char op;
    while (q--) {
        cin >> op;
        int x, y;
        cin >> x >> y;
        if (op == 'U' || op == 'u')
            dsu.unite(x, y);
        else
            cout << (dsu.same(x, y) ? "YES" : "NO") << '\n';
    }
    return 0;
}
