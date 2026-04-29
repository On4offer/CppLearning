/*
 * STL Demo：<numeric> 数值算法
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== iota：连续填充 ===\n";
    vector<int> a(5);
    iota(a.begin(), a.end(), 10);  // 10,11,12,13,14
    for (int x : a) cout << x << " ";
    cout << "\n";

    cout << "=== accumulate ===\n";
    cout << "sum=" << accumulate(a.begin(), a.end(), 0LL) << "\n";
    cout << "product=" << accumulate(a.begin(), a.end(), 1LL, multiplies<long long>()) << "\n";

    cout << "=== partial_sum 前缀和 ===\n";
    vector<int> b = {1, 2, 3, 4};
    vector<int> pref(b.size());
    partial_sum(b.begin(), b.end(), pref.begin());
    for (int x : pref) cout << x << " ";
    cout << "\n";

    cout << "=== inner_product 点积 ===\n";
    vector<int> x = {1, 2, 3}, y = {4, 5, 6};
    cout << "dot=" << inner_product(x.begin(), x.end(), y.begin(), 0) << "\n";

    cout << "=== adjacent_difference 相邻差分 ===\n";
    vector<int> d = {5, 9, 12, 20};
    vector<int> diff(d.size());
    adjacent_difference(d.begin(), d.end(), diff.begin());
    for (int v : diff) cout << v << " ";
    cout << "\n";

    cout << "=== gcd / lcm（C++17，需 <numeric>）===\n";
    cout << "gcd(12,18)=" << gcd(12, 18) << " lcm=" << lcm(12, 18) << "\n";

    return 0;
}
