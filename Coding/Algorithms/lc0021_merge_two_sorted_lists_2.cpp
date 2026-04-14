/*
 * 21. 合并两个有序链表（解法二）
 *
 * 解法：递归。merge(a,b) 若 a 空返回 b；若 b 空返回 a；
 * 否则较小者作为头，其 next 指向 merge(较小者->next, 另一表)。
 * 时间 O(n+m)，空间 O(n+m) 递归栈。
 *
 * 输入/输出：同 lc0021_merge_two_sorted_lists.cpp
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// ============================================================================
// 核心方法（题号 21，递归）
// ============================================================================
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (!list1) return list2;
    if (!list2) return list1;
    if (list1->val <= list2->val) {
        list1->next = mergeTwoLists(list1->next, list2);
        return list1;
    }
    list2->next = mergeTwoLists(list1, list2->next);
    return list2;
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
