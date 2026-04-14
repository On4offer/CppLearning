/*
 * 手撕：Dijkstra 最短路（非负权边）
 *
 * 输入：第一行 n m s，接下来 m 行 u v w（有向边）
 * 输出：s 到各点最短路，不可达 -1
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法
// ============================================================================
vector<long long> dijkstra(int n, const vector<vector<pair<int, int>>>& g, int s) {
    const long long INF = (1LL << 60);
    vector<long long> dist(n, INF);
    dist[s] = 0;
    using P = pair<long long, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
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
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }
    const long long INF = (1LL << 60);
    auto dist = dijkstra(n, g, s);
    for (int i = 0; i < n; ++i) {
        if (dist[i] >= INF / 2)
            cout << -1;
        else
            cout << dist[i];
        cout << '\n';
    }
    return 0;
}
