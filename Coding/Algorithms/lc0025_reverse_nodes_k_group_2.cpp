/*
 * 25. K 个一组翻转链表（解法二）
 *
 * 解法：递归。reverseKGroup(head,k) 先判断本段是否有 k 个结点；
 * 若有则递归处理后面链表，再对本段做普通链表翻转并拼接。
 * 时间 O(n)，空间 O(n/k) 递归栈。
 *
 * 输入/输出：同 lc0025_reverse_nodes_k_group.cpp
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

namespace {
pair<ListNode*, ListNode*> reverseRange(ListNode* a, ListNode* b) {
    ListNode* pre = nullptr;
    ListNode* cur = a;
    while (cur != b) {
        ListNode* nx = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nx;
    }
    return {pre, a};
}
}  // namespace

// ============================================================================
// 核心方法（题号 25，递归）
// ============================================================================
ListNode* reverseKGroup(ListNode* head, int k) {
    if (!head) return nullptr;
    ListNode* b = head;
    for (int i = 0; i < k; ++i) {
        if (!b) return head;
        b = b->next;
    }
    auto pr = reverseRange(head, b);
    ListNode* newHead = pr.first;
    ListNode* tail = pr.second;
    tail->next = reverseKGroup(b, k);
    return newHead;
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
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    ListNode* dummy = new ListNode(0);
    ListNode* t = dummy;
    for (int x : a) {
        t->next = new ListNode(x);
        t = t->next;
    }
    ListNode* head = reverseKGroup(dummy->next, k);
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
