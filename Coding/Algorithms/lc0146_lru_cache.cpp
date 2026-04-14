/*
 * 146. LRU 缓存
 * 实现容量为 capacity 的 LRU：get(key) 若存在返回 value 并刷新为最近使用；否则 -1。
 * put(key,value)：若已满则淘汰最久未使用，再插入/更新。
 *
 * 解法：双向链表维护使用顺序（头最近、尾最久）+ 哈希表 key -> 迭代器/节点指针。
 * get/put 均为 O(1) 均摊。
 *
 * 输入：
 *   第一行 capacity
 *   第二行 q（操作数）
 *   接下来 q 行：G key 表示 get；P key val 表示 put
 * 输出：每个 G 对应一行，为查询结果；-1 表示未命中。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心：题号 146（LRUCache 类与接口与题面一致；粘贴时保留本类即可）
// ============================================================================
struct Node {
    int key, val;
    Node *prev, *next;
    Node(int k = 0, int v = 0) : key(k), val(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
    int cap;
    unordered_map<int, Node*> mp;
    Node *head, *tail;

    void remove(Node* x) {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }
    void addFront(Node* x) {
        x->next = head->next;
        x->prev = head;
        head->next->prev = x;
        head->next = x;
    }

public:
    explicit LRUCache(int capacity) : cap(capacity) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    ~LRUCache() {
        for (auto& [k, p] : mp) delete p;
        delete head;
        delete tail;
    }

    int get(int key) {
        auto it = mp.find(key);
        if (it == mp.end()) return -1;
        Node* x = it->second;
        remove(x);
        addFront(x);
        return x->val;
    }

    void put(int key, int value) {
        if (mp.count(key)) {
            Node* x = mp[key];
            x->val = value;
            remove(x);
            addFront(x);
            return;
        }
        if ((int)mp.size() == cap) {
            Node* old = tail->prev;
            remove(old);
            mp.erase(old->key);
            delete old;
        }
        Node* x = new Node(key, value);
        mp[key] = x;
        addFront(x);
    }
};

// ============================================================================
// ACM：读入操作序列 → 调用 LRUCache → 输出（无额外类名要求）
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int cap, q;
    if (!(cin >> cap >> q)) return 0;
    LRUCache cache(cap);
    char op;
    for (int i = 0; i < q; ++i) {
        cin >> op;
        if (op == 'G' || op == 'g') {
            int k;
            cin >> k;
            cout << cache.get(k) << '\n';
        } else {
            int k, v;
            cin >> k >> v;
            cache.put(k, v);
        }
    }
    return 0;
}
