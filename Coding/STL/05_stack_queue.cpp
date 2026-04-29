/*
 * STL Demo：stack、queue、priority_queue
 *
 * stack：LIFO，默认 deque 为底层
 * queue：FIFO，默认 deque
 * priority_queue：大顶堆（默认 less，堆顶最大）；小顶堆用 greater 或反号
 */
#include <bits/stdc++.h>
using namespace std;

void demo_stack() {
    cout << "=== stack ===\n";
    stack<int> st;
    st.push(1);
    st.push(2);
    cout << "top=" << st.top() << " size=" << st.size() << "\n";
    st.pop();
    cout << "after pop top=" << st.top() << "\n";
}

void demo_queue() {
    cout << "=== queue ===\n";
    queue<int> q;
    q.push(1);
    q.push(2);
    cout << "front=" << q.front() << " back=" << q.back() << "\n";
    q.pop();
}

void demo_priority_queue() {
    cout << "=== priority_queue (max-heap) ===\n";
    priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << "\n";

    cout << "=== min-heap (greater) ===\n";
    priority_queue<int, vector<int>, greater<int>> pqmin;
    pqmin.push(3);
    pqmin.push(1);
    pqmin.push(4);
    while (!pqmin.empty()) {
        cout << pqmin.top() << " ";
        pqmin.pop();
    }
    cout << "\n";

    // 第 k 大：维护大小为 k 的小顶堆（与算法题常见）
    cout << "=== 第 2 大（示例数组）===\n";
    vector<int> a = {5, 1, 9, 2};
    int k = 2;
    priority_queue<int, vector<int>, greater<int>> kpq;
    for (int x : a) {
        kpq.push(x);
        if ((int)kpq.size() > k) kpq.pop();
    }
    cout << k << "-th largest: " << kpq.top() << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    demo_stack();
    demo_queue();
    demo_priority_queue();
    return 0;
}
