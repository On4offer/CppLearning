/*
 * STL Demo：<algorithm> 常用算法
 * 多数要求随机访问迭代器（如 vector/array）；list 需用成员函数或 copy 到 vector 再排。
 */
#include <bits/stdc++.h>
using namespace std;

void print_vec(const string& name, const vector<int>& a) {
    cout << name << ": ";
    for (int x : a) cout << x << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== sort / stable_sort ===\n";
    vector<int> a = {3, 1, 4, 1, 5};
    sort(a.begin(), a.end());
    print_vec("sort asc", a);
    sort(a.begin(), a.end(), greater<int>());
    print_vec("sort desc", a);

    cout << "=== nth_element（第 k 小放位置 k，左右不保证有序）===\n";
    a = {3, 1, 4, 1, 5};
    nth_element(a.begin(), a.begin() + 2, a.end());
    print_vec("nth k=2", a);
    cout << "a[2]=" << a[2] << "\n";

    cout << "=== partial_sort（前 k 个为最小 k 个，升序）===\n";
    a = {3, 1, 4, 1, 5};
    partial_sort(a.begin(), a.begin() + 3, a.end());
    print_vec("partial_sort k=3", a);

    cout << "=== 二分（需已排序）===\n";
    a = {1, 2, 2, 2, 5};
    auto it = lower_bound(a.begin(), a.end(), 2);
    auto it2 = upper_bound(a.begin(), a.end(), 2);
    cout << "lower_bound(2) idx=" << (it - a.begin()) << " upper idx=" << (it2 - a.begin())
         << " count=" << (it2 - it) << "\n";
    cout << "binary_search(5)=" << binary_search(a.begin(), a.end(), 5) << "\n";

    cout << "=== unique（先 sort 再去重，erase 收缩）===\n";
    a = {1, 1, 2, 2, 3};
    auto last = unique(a.begin(), a.end());
    a.erase(last, a.end());
    print_vec("unique", a);

    cout << "=== reverse / rotate ===\n";
    a = {1, 2, 3, 4, 5};
    reverse(a.begin(), a.end());
    print_vec("reverse", a);
    a = {1, 2, 3, 4, 5};
    rotate(a.begin(), a.begin() + 2, a.end());
    print_vec("rotate left 2", a);

    cout << "=== next_permutation / prev_permutation ===\n";
    a = {1, 2, 3};
    int cnt = 0;
    do {
        ++cnt;
        if (cnt <= 3) print_vec("perm", a);
    } while (next_permutation(a.begin(), a.end()));
    cout << "total permutations of 3 elements: " << cnt << "\n";

    cout << "=== min_element / max_element / minmax ===\n";
    a = {3, 1, 4, 1, 5};
    cout << "min=" << *min_element(a.begin(), a.end()) << "\n";
    auto mm = minmax_element(a.begin(), a.end());
    cout << "minmax: " << *mm.first << " " << *mm.second << "\n";

    cout << "=== all_of / any_of / count ===\n";
    a = {2, 4, 6};
    cout << "all even? " << all_of(a.begin(), a.end(), [](int x) { return x % 2 == 0; }) << "\n";
    vector<int> c = {3, 1, 4, 1, 5};
    cout << "count(1) in {3,1,4,1,5}: " << count(c.begin(), c.end(), 1) << "\n";

    cout << "=== copy_if ===\n";
    a = {1, 2, 3, 4, 5};
    vector<int> b;
    copy_if(a.begin(), a.end(), back_inserter(b), [](int x) { return x % 2 == 0; });
    print_vec("evens", b);

    cout << "=== merge（两序列已有序）===\n";
    vector<int> L = {1, 3, 5}, R = {2, 4, 6};
    vector<int> out(L.size() + R.size());
    merge(L.begin(), L.end(), R.begin(), R.end(), out.begin());
    print_vec("merge", out);

    cout << "=== partition ===\n";
    a = {1, 2, 3, 4, 5};
    auto mid = partition(a.begin(), a.end(), [](int x) { return x % 2 == 0; });
    print_vec("partition even first", a);
    cout << "first odd at idx " << (mid - a.begin()) << "\n";

    return 0;
}
