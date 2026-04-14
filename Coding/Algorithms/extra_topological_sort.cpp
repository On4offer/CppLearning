/*
 * 手撕：拓扑排序（Kahn）
 * 有向无环图输出拓扑序；有环则返回空 vector。
 *
 * 输入：第一行 n m，接下来 m 行 u v（有向边 u→v）
 * 输出：拓扑序或 NO
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法
// ============================================================================
vector<int> topologicalSort(int n, const vector<pair<int, int>>& edges) {
    vector<vector<int>> g(n);
    vector<int> indeg(n, 0);
    for (auto [u, v] : edges) {
        g[u].push_back(v);
        ++indeg[v];
    }
    queue<int> q;
    for (int i = 0; i < n; ++i)
        if (indeg[i] == 0) q.push(i);
    vector<int> order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }
    if ((int)order.size() != n) return {};
    return order;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) cin >> edges[i].first >> edges[i].second;
    auto order = topologicalSort(n, edges);
    if (order.empty()) {
        cout << "NO\n";
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << order[i];
    }
    cout << '\n';
    return 0;
}
