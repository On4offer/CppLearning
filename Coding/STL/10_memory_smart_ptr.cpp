/*
 * STL Demo：<memory> 智能指针（资源管理，非「容器」但常与 STL 一起学）
 * unique_ptr：独占；shared_ptr：共享引用计数；weak_ptr：不延长寿命的弱引用。
 */
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    shared_ptr<Node> next;
    explicit Node(int v) : val(v) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== unique_ptr ===\n";
    unique_ptr<int> p(new int(42));
    cout << *p << "\n";
    auto q = make_unique<int>(7);  // C++14 起推荐
    cout << *q << "\n";
    p = move(q);  // 所有权转移
    cout << "after move p=" << *p << " q=" << (q ? "set" : "null") << "\n";

    cout << "=== shared_ptr ===\n";
    shared_ptr<Node> a = make_shared<Node>(1);
    shared_ptr<Node> b = make_shared<Node>(2);
    a->next = b;
    cout << "a.use_count=" << a.use_count() << " b.use_count=" << b.use_count() << "\n";
    {
        shared_ptr<Node> c = b;
        cout << "with c: b.use_count=" << b.use_count() << "\n";
    }
    cout << "c out of scope: b.use_count=" << b.use_count() << "\n";

    cout << "=== weak_ptr（避免环引用导致泄漏时可打断环）===\n";
    weak_ptr<Node> w = b;
    cout << "w.expired=" << w.expired() << "\n";
    if (auto s = w.lock()) cout << "lock->val=" << s->val << "\n";

    return 0;
}
