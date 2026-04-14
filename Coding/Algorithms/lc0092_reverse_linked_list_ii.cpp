/*
 * 92. 反转链表 II
 * 反转单链表中从位置 left 到 right（含）的结点（1-indexed）。
 *
 * 解法：头插法。定位 left 的前驱 pre，将 [left+1..right] 依次摘下插到 pre 之后。
 * 共 right-left 次。时间 O(n)，空间 O(1)。
 *
 * 输入：
 *   第一行 n、left、right（1<=left<=right<=n）
 *   第二行 n 个整数
 * 输出：反转区间后的序列。
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// ============================================================================
// 核心方法（题号 92）
// ============================================================================
ListNode* reverseBetween(ListNode* head, int left, int right) {
    auto dummy = new ListNode(0, head);
    ListNode* pre = dummy;
    for (int i = 1; i < left; ++i) pre = pre->next;
    ListNode* cur = pre->next;
    for (int i = 0; i < right - left; ++i) {
        ListNode* nx = cur->next;
        cur->next = nx->next;
        nx->next = pre->next;
        pre->next = nx;
    }
    ListNode* res = dummy->next;
    delete dummy;
    return res;
}

static void deleteList(ListNode* h) {
    while (h) {
        ListNode* nx = h->next;
        delete h;
        h = nx;
    }
}

// ============================================================================
// ACM：建表 → 调用 → 打印 → 释放
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, left, right;
    if (!(cin >> n >> left >> right)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    ListNode* dummy = new ListNode(0);
    ListNode* t = dummy;
    for (int x : a) {
        t->next = new ListNode(x);
        t = t->next;
    }
    ListNode* head = reverseBetween(dummy->next, left, right);
    delete dummy;
    bool first = true;
    for (ListNode* p = head; p; p = p->next) {
        if (!first) cout << ' ';
        first = false;
        cout << p->val;
    }
    cout << '\n';
    deleteList(head);
    return 0;
}
