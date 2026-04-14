/*
 * 102. 二叉树的层序遍历
 * 自上而下逐层、从左到右输出结点值。
 *
 * 解法：BFS 队列。根入队；每层处理当前队列长度个结点，子结点入队。
 * 时间 O(n)，空间 O(队列最大宽度)。
 *
 * 输入：
 *   一行：按层序给出结点，空格分隔，# 表示空位（与 LeetCode 数组表示一致）。
 *   例：3 9 20 # # 15 7
 * 输出：
 *   每层一行，结点值空格分隔。
 */
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ============================================================================
// 核心方法（题号 102）
// ============================================================================
vector<vector<int>> levelOrder(TreeNode* root) {
    if (!root) return {};
    vector<vector<int>> ans;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = (int)q.size();
        vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* u = q.front();
            q.pop();
            level.push_back(u->val);
            if (u->left) q.push(u->left);
            if (u->right) q.push(u->right);
        }
        ans.push_back(move(level));
    }
    return ans;
}

static TreeNode* buildLevelOrder(const vector<string>& tok) {
    if (tok.empty() || tok[0] == "#") return nullptr;
    int idx = 0;
    auto root = new TreeNode(stoi(tok[idx++]));
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty() && idx < (int)tok.size()) {
        TreeNode* u = q.front();
        q.pop();
        if (idx < (int)tok.size()) {
            if (tok[idx] != "#") {
                u->left = new TreeNode(stoi(tok[idx]));
                q.push(u->left);
            }
            ++idx;
        }
        if (idx < (int)tok.size()) {
            if (tok[idx] != "#") {
                u->right = new TreeNode(stoi(tok[idx]));
                q.push(u->right);
            }
            ++idx;
        }
    }
    return root;
}

static void freeTree(TreeNode* r) {
    if (!r) return;
    freeTree(r->left);
    freeTree(r->right);
    delete r;
}

// ============================================================================
// ACM：层序字符串建树 → 调用 → 按层打印 → 释放
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    getline(cin, line);
    stringstream ss(line);
    vector<string> tok;
    string w;
    while (ss >> w) tok.push_back(w);
    TreeNode* root = buildLevelOrder(tok);
    auto levels = levelOrder(root);
    bool firstLine = true;
    for (auto& level : levels) {
        if (!firstLine) cout << '\n';
        firstLine = false;
        for (int i = 0; i < (int)level.size(); ++i) {
            if (i) cout << ' ';
            cout << level[i];
        }
    }
    cout << '\n';
    freeTree(root);
    return 0;
}
