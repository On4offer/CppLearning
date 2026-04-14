/*
 * 手撕：BFS 无权图最短路（边数最少）
 *
 * 输入：第一行 n m s，接下来 m 行 u v（无向边）
 * 输出：s 到各点最短边数，不可达 -1
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法
// ============================================================================
vector<int> bfsDist(int n, const vector<vector<int>>& g, int s) {
    vector<int> dist(n, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s;
    if (!(cin >> n >> m >> s)) return 0;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto dist = bfsDist(n, g, s);
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << dist[i];
    }
    cout << '\n';
    return 0;
}
