/*
 * STL Demo：迭代器基础
 * 类别：随机访问（vector）、双向（list/set）、前向（forward_list/unordered_*）。
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== begin / end 与下标 ===\n";
    vector<int> a = {10, 20, 30};
    for (auto it = a.begin(); it != a.end(); ++it) cout << *it << " ";
    cout << "\n";
    cout << "distance(begin, begin+2)=" << distance(a.begin(), a.begin() + 2) << "\n";

    cout << "=== reverse_iterator（逆序遍历，注意 ++ 向头部走）===\n";
    for (auto it = a.rbegin(); it != a.rend(); ++it) cout << *it << " ";
    cout << "\n";

    cout << "=== advance / next / prev ===\n";
    auto it = a.begin();
    advance(it, 2);
    cout << "*advance(begin,2)=" << *it << "\n";
    cout << "*next(begin)=" << *next(a.begin()) << "\n";
    cout << "*prev(end-1)=" << *prev(a.end()) << "\n";

    cout << "=== insert 迭代器（vector insert 可能使迭代器失效，演示在尾部前插）===\n";
    a.insert(a.begin() + 1, 15);
    for (int x : a) cout << x << " ";
    cout << "\n";

    cout << "=== 原生数组与 begin/end ===\n";
    int arr[] = {7, 8, 9};
    sort(begin(arr), end(arr));
    for (int x : arr) cout << x << " ";
    cout << "\n";

    cout << "=== list：仅双向迭代器，无 it+n ===\n";
    list<int> L = {1, 2, 3};
    auto lit = L.begin();
    ++lit;
    // lit + 2 非法；用 advance(lit, 2)
    advance(lit, 1);
    cout << "*list second: " << *lit << "\n";

    return 0;
}
