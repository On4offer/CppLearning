/*
 * STL Demo：set / multiset、map / multimap
 *
 * 底层多为红黑树，有序；迭代器遍历为升序（默认 less<T>）。
 * set：键唯一；multiset：键可重复。
 * map：键唯一，mapped_type 为值；multimap：同一键可多条。
 */
#include <bits/stdc++.h>    // 引入头文件,包含所有标准库头文件
using namespace std;

void demo_set() {
    cout << "=== set ===\n";
    set<int> S = {3, 1, 4, 1}; // 初始化，重复的1会被忽略
    cout << "size=" << S.size() << " (duplicate 1 dropped)\n"; // 输出集合的大小，重复的1会被忽略
    S.insert(2);
    if (S.count(3)) cout << "contains 3\n"; // 输出集合中是否包含3
    auto it = S.lower_bound(2); // 返回集合中第一个大于等于2的元素的迭代器，返回的是迭代器，相当于一个指针，指向集合中第一个大于等于2的元素
    cout << "lower_bound(2)=" << *it << "\n"; // 输出集合中第一个大于等于2的元素的值，*it相当于解引用，输出的是集合中第一个大于等于2的元素的值
}

// multiset是允许重复元素的集合，底层实现是红黑树，有序
void demo_multiset() {
    cout << "=== multiset ===\n";
    multiset<int> MS;
    MS.insert(1);
    MS.insert(1);
    MS.insert(2);
    cout << "count(1)=" << MS.count(1) << "\n";
}

void demo_map() {
    cout << "=== map ===\n";
    map<string, int> mp;
    mp["apple"] = 3;
    mp["banana"] = 2;
    mp.insert({"pear", 5});
    for (auto& [k, v] : mp) cout << k << ":" << v << " ";
    cout << "\n";
    // 注意：mp[key] 若 key 不存在会插入默认值
    cout << "mp[\"none\"] -> " << mp["none"] << " (inserted default)\n";
}

void demo_multimap() {
    cout << "=== multimap ===\n";
    multimap<int, string> mm;
    mm.insert({1, "a"});
    mm.insert({1, "b"});
    auto range = mm.equal_range(1);
    for (auto it = range.first; it != range.second; ++it)
        cout << it->second << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    demo_set();
    demo_multiset();
    demo_map();
    demo_multimap();
    return 0;
}
