/*
 * 21. 合并两个有序链表
 * 将两个升序链表合并为一个升序链表。
 *
 * 解法：迭代哑结点 pre，比较两表头较小者接到 pre 后，直至某一表空，再接上剩余。
 * 时间 O(n+m)，空间 O(1)。
 *
 * 输入：
 *   第一行 n m（两表长度）
 *   第二行 n 个整数（第一表）
 *   第三行 m 个整数（第二表）
 * 输出：合并后序列，空格分隔。
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// ============================================================================
// 核心方法（题号 21）
// ============================================================================
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy(0);
    ListNode* t = &dummy;
    ListNode *a = list1, *b = list2;
    while (a && b) {
        if (a->val <= b->val) {
            t->next = a;
            a = a->next;
        } else {
            t->next = b;
            b = b->next;
        }
        t = t->next;
    }
    t->next = a ? a : b;
    return dummy.next;
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
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> A(n), B(m);
    for (int& x : A) cin >> x;
    for (int& x : B) cin >> x;
    ListNode *da = new ListNode(0), *db = new ListNode(0);
    ListNode *ta = da, *tb = db;
    for (int x : A) {
        ta->next = new ListNode(x);
        ta = ta->next;
    }
    for (int x : B) {
        tb->next = new ListNode(x);
        tb = tb->next;
    }
    ListNode* head = mergeTwoLists(da->next, db->next);
    delete da;
    delete db;
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
