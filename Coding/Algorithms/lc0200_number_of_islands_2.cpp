/*
 * 200. 岛屿数量（解法二）
 *
 * 解法：BFS。对每个陆地单元格入队，扩展四邻未访问陆地。
 * 时间 O(mn)，空间 O(min(mn, 队列规模))。
 *
 * 输入/输出：同 lc0200_number_of_islands.cpp
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 200，BFS）
// ============================================================================
int numIslands(vector<vector<char>>& grid) {
    int m = (int)grid.size();
    if (m == 0) return 0;
    int n = (int)grid[0].size();
    vector<vector<char>> vis(m, vector<char>(n, 0));
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    int ans = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] != '1' || vis[i][j]) continue;
            ++ans;
            queue<pair<int, int>> q;
            q.push({i, j});
            vis[i][j] = 1;
            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                for (int k = 0; k < 4; ++k) {
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
                    if (grid[nr][nc] != '1' || vis[nr][nc]) continue;
                    vis[nr][nc] = 1;
                    q.push({nr, nc});
                }
            }
        }
    return ans;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n;
    if (!(cin >> m >> n)) return 0;
    vector<string> rows(m);
    for (int i = 0; i < m; ++i) cin >> rows[i];
    vector<vector<char>> grid(m, vector<char>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) grid[i][j] = rows[i][j];
    cout << numIslands(grid) << '\n';
    return 0;
}
