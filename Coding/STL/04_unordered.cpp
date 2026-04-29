/*
 * STL Demo：unordered_set、unordered_map
 *
 * 哈希表：平均 O(1) 插入/查找/删除；最坏可退化（需好哈希与预留桶）。
 * 元素无序；键需要可哈希（自定义类型要提供 hash 与 operator==）。
 */
#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

struct PointHash {
    size_t operator()(const Point& p) const {
        return ((size_t)p.x << 16) ^ (unsigned)p.y;
    }
};

void demo_unordered() {
    cout << "=== unordered_set / unordered_map ===\n";
    unordered_set<int> us = {1, 2, 3};
    us.insert(2);
    cout << "size=" << us.size() << "\n";
    us.reserve(100);  // 减少 rehash 次数（已知大致大小时）

    unordered_map<string, int> ump;
    ump["a"] = 1;
    ump.emplace("b", 2);
    for (auto& [k, v] : ump) cout << k << ":" << v << " ";
    cout << "\n";

    unordered_set<Point, PointHash> pts;
    pts.insert({1, 2});
    pts.insert({1, 2});
    cout << "Point set size=" << pts.size() << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    demo_unordered();
    return 0;
}
