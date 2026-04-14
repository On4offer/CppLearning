/*
 * 143. 重排链表
 * L0→L1→…→Ln-1 变为 L0→Ln-1→L1→Ln-2→…
 *
 * 解法：找中点（快慢指针）拆分；反转后半段；交替合并两段。
 * 时间 O(n)，空间 O(1)。
 *
 * 输入：
 *   第一行 n（结点个数）
 *   第二行 n 个整数
 * 输出：重排后序列。
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

namespace {
ListNode* reverseList(ListNode* head) {
    ListNode* pre = nullptr, *cur = head;
    while (cur) {
        ListNode* nx = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nx;
    }
    return pre;
}
}  // namespace

// ============================================================================
// 核心方法（题号 143）
// ============================================================================
void reorderList(ListNode* head) {
    if (!head || !head->next) return;
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* mid = slow->next;
    slow->next = nullptr;
    ListNode* B = reverseList(mid);
    ListNode* A = head;
    while (A && B) {
        ListNode* a1 = A->next;
        ListNode* b1 = B->next;
        A->next = B;
        B->next = a1;
        A = a1;
        B = b1;
    }
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
    reorderList(dummy->next);
    bool first = true;
    for (ListNode* p = dummy->next; p; p = p->next) {
        if (!first) cout << ' ';
        first = false;
        cout << p->val;
    }
    cout << '\n';
    deleteList(dummy->next);
    delete dummy;
    return 0;
}
