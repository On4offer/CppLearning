/*
 * STL Demo：deque、list、forward_list
 *
 * deque：双端队列，头尾插入删除 O(1)，随机访问 O(1) 但常数通常比 vector 大。
 * list：双向链表，插入删除 O(1)，无随机访问，迭代器仅 ++/--。
 * forward_list：单向链表，更省内存，操作略别扭（常带 after 参数）。
 */
#include <bits/stdc++.h>
using namespace std;

void demo_deque() {
    cout << "=== deque ===\n";
    deque<int> d;
    d.push_back(1); // 尾部添加元素
    d.push_front(0); // 头部添加元素
    d.push_back(2); // 尾部添加元素
    for (int x : d) cout << x << " ";
    cout << "\n";
    cout << "d[1]=" << d[1] << "\n";
}

void demo_list() {
    cout << "=== list ===\n";
    list<int> L = {1, 2, 3};
    auto it = L.begin(); // 获取第一个元素的迭代器
    ++it; // 移动到第二个元素
    L.insert(it, 99);  // 在 it 前插入，it 仍有效
    for (int x : L) cout << x << " "; // 遍历链表
    cout << "\n";
    L.remove(99);  // 按值删除所有 99，时间复杂度O(n)
    for (int x : L) cout << x << " "; // 遍历链表
    cout << "\n";
}
// forward_list是单向链表，只能向前遍历，不能向后遍历
void demo_forward_list() {
    cout << "=== forward_list ===\n";
    forward_list<int> F = {1, 2, 3};
    F.push_front(0); // 头部添加元素
    auto after = F.before_begin(); // 获取第一个元素的前一个位置的迭代器
    F.insert_after(after, -1);  // 在首元素前插入
    for (int x : F) cout << x << " "; // 遍历链表
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false); // 关闭同步，提高输入输出效率+解除 cin 与 cout 的绑定，提高输入输出效率
    cin.tie(nullptr);
    demo_deque(); // 演示 deque
    demo_list(); // 演示 list
    demo_forward_list(); // 演示 forward_list
    return 0;
}
