/*
 * 200. 岛屿数量
 * 二维网格由 '1'（陆地）和 '0'（水）组成，四连通统计岛屿个数。
 *
 * 解法：DFS。遍历每个未访问的陆地，标记与其相连区域为已访问，计数++。
 * 时间 O(mn)，空间 O(mn) 递归栈最坏。
 *
 * 输入：
 *   第一行 m n
 *   接下来 m 行，每行 n 个字符 0/1 无空格
 * 输出：岛屿数量。
 */
#include <bits/stdc++.h>
using namespace std;

namespace {
void dfs(vector<vector<char>>& g, vector<vector<char>>& vis, int r, int c) {
    int m = (int)g.size(), n = (int)g[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || g[r][c] != '1' || vis[r][c]) return;
    vis[r][c] = 1;
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    for (int k = 0; k < 4; ++k) dfs(g, vis, r + dr[k], c + dc[k]);
}
}  // namespace

// ============================================================================
// 核心方法（题号 200，DFS）
// ============================================================================
int numIslands(vector<vector<char>>& grid) {
    int m = (int)grid.size();
    if (m == 0) return 0;
    int n = (int)grid[0].size();
    vector<vector<char>> vis(m, vector<char>(n, 0));
    int ans = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] == '1' && !vis[i][j]) {
                ++ans;
                dfs(grid, vis, i, j);
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
