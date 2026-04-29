/*
 * STL Demo：vector、array
 *
 * vector：动态数组，连续内存，随机访问 O(1)，尾部插入均摊 O(1)。
 * array：定长、栈上友好，大小编译期确定。
 */
#include <bits/stdc++.h>    // 万能头文件, 包含所有标准库头文件, 不推荐使用
using namespace std;

void demo_vector() {
    cout << "=== vector ===\n";
    vector<int> a = {3, 1, 4};  // 初始化，也可以用a.resize(3)来初始化
    a.push_back(1); // 尾部添加元素
    // a.emplace_back(5); 的意思是：在 vector 的末尾再放进一个元素，这个元素用参数 5 在尾部槽位上直接构造出来。
    a.emplace_back(5);  // 原地构造，避免多余拷贝（对复杂类型更有用）
    // size是实际元素个数，capacity是实际分配的内存容量
    // size = 5 是因为我们添加了5个元素，capacity = 6 是因为 vector 会预留一些空间，避免频繁扩容
    cout << "size=" << a.size() << " capacity=" << a.capacity() << "\n";

    sort(a.begin(), a.end());   // 排序，时间复杂度O(nlogn)，最坏情况O(n^2)，最好情况O(n)
    for (int x : a) cout << x << " ";
    cout << "\n";

    // 二分（需已排序）
    // lower_bound(首, 尾, 值) 做二分，在 [begin, end) 里找第一个满足「不小于 3」 的位置（默认用 <：即 *it < 3 为假，等价于 *it >= 3）。
    auto it = lower_bound(a.begin(), a.end(), 3);
    cout << "lower_bound(3) index: " << (it - a.begin()) << "\n";

    // 删除：按值删一个（需 <algorithm> remove + erase 惯用法）
    a = {1, 2, 3, 2, 4};
    a.erase(remove(a.begin(), a.end(), 2), a.end());
    cout << "after remove all 2: ";
    for (int x : a) cout << x << " ";
    cout << "\n";

    // 二维：注意每一行可以是不同长度
    vector<vector<int>> g(3);
    g[0] = {1, 2};
    g[1] = {3};
    cout << "g[0].size()=" << g[0].size() << "\n";
}

// 这是定长数组，长度在编译期确定，不能动态扩展，但是可以修改元素
void demo_array() {
    cout << "=== array ===\n";
    // array<类型, 长度> 变量名 = {值1, 值2, 值3, ...};
    // 长度在编译期确定，不能动态扩展，但是可以修改元素
    // 访问元素：变量名[下标] 或 at(下标)
    array<int, 4> b = {9, 8, 7, 6};
    cout << "b[0]=" << b[0] << " at()=" << b.at(1) << "\n";
    cout << "size (compile-time): " << b.size() << "\n";
}

int main() {
    // 关闭同步，提高输入输出效率
    ios::sync_with_stdio(false);
    // 解除 cin 与 cout 的绑定，提高输入输出效率
    cin.tie(nullptr);
    demo_vector();  // 演示 vector
    demo_array();   // 演示 array
    return 0;       // 返回 0，表示程序正常结束
}
