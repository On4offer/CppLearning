/*
 * 25. K 个一组翻转链表
 * 将链表每 k 个结点为一组进行翻转，剩余不足 k 个保持顺序。
 *
 * 解法：迭代。每段先数满 k 个结点；用头插法翻转该段；连接前段尾与下段头。
 * 时间 O(n)，空间 O(1)。
 *
 * 输入：
 *   第一行 n（结点个数）、k
 *   第二行 n 个整数（链表从左到右）
 * 输出：一行，翻转后的链表序列。
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// ============================================================================
// 核心方法（题号 25）
// ============================================================================
ListNode* reverseKGroup(ListNode* head, int k) {
    auto dummy = new ListNode(0, head);
    ListNode* pre = dummy;
    while (true) {
        ListNode* kth = pre;
        for (int i = 0; i < k; ++i) {
            kth = kth->next;
            if (!kth) {
                ListNode* res = dummy->next;
                delete dummy;
                return res;
            }
        }
        ListNode* first = pre->next;
        for (int i = 0; i < k - 1; ++i) {
            ListNode* nex = first->next;
            first->next = nex->next;
            nex->next = pre->next;
            pre->next = nex;
        }
        pre = first;
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
