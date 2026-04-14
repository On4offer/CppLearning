/*
 * 206. 反转链表
 * 将单链表结点顺序反转。
 *
 * 解法：迭代。用 pre/cur 扫描，每次把 cur->next 指向前驱。
 * 时间 O(n)，空间 O(1)。
 *
 * 输入：
 *   第一行 n（结点个数）
 *   第二行 n 个整数
 * 输出：反转后序列，空格分隔。
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// ============================================================================
// 核心方法（题号 206）
// ============================================================================
ListNode* reverseList(ListNode* head) {
    ListNode* pre = nullptr;
    ListNode* cur = head;
    while (cur) {
        ListNode* nx = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nx;
    }
    return pre;
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
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    ListNode* dummy = new ListNode(0);
    ListNode* t = dummy;
    for (int x : a) {
        t->next = new ListNode(x);
        t = t->next;
    }
    ListNode* head = reverseList(dummy->next);
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
