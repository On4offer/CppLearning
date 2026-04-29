/*
 * 图中知识点 Demo：
 * 1) 指针和引用的区别
 * 2) 常量指针（pointer to const）：const int* p
 * 3) 指针常量（const pointer）：int* const p
 * 4) const int* const p：指向常量的常量指针
 */
#include <bits/stdc++.h>
using namespace std;

void demoReferenceVsPointer() {
    cout << "=== 1) 引用 vs 指针 ===\n";

    int a = 10, b = 20;

    int& ref = a;   // 引用必须初始化，且之后不能改绑到别的变量
    int* ptr = &a;  // 指针可以改为指向其他地址

    cout << "初始: a=" << a << ", b=" << b << "\n";
    ref = 11;  // 修改 a
    cout << "ref = 11 后: a=" << a << "\n";

    ptr = &b;  // 改变指向
    *ptr = 21; // 修改 b
    cout << "ptr 指向 b 并 *ptr=21 后: b=" << b << "\n";

    // 错误示例（仅注释，不执行）：
    // int& badRef;      // error: 引用必须初始化
    // int& ref2 = a;
    // ref2 = b;         // 这是赋值，不是改绑引用

    cout << "\n";
}

void demoPointerToConst() {
    cout << "=== 2) 常量指针: const int* p (或 int const* p) ===\n";

    int x = 5, y = 8;
    const int* p = &x;  // p是一个常量指针，指向x，p的值为x的地址

    cout << "初始: x=" << x << ", y=" << y << ", *p=" << *p << "\n";

    // *p = 100; // error: 不能通过 p 修改其指向内容
    p = &y;      // 可以改指向
    cout << "p 改指向 y 后: *p=" << *p << "\n";

    y = 88;      // 对象本身若非 const，仍可通过原变量修改
    cout << "y=88 后: *p=" << *p << "\n";

    cout << "结论: 指向可变，指向内容通过该指针不可变。\n\n";
}

void demoConstPointer() {
    cout << "=== 3) 指针常量: int* const p ===\n";

    int x = 3;
    int y = 9;
    int* const p = &x; // p 自身不可改指向，必须初始化

    cout << "初始: x=" << x << ", y=" << y << ", *p=" << *p << "\n";

    *p = 33; // 可以改所指向内容
    cout << "*p=33 后: x=" << x << "\n";

    // p = &y; // error: p 是常量指针，不能改指向
    (void)y;  // 避免未使用警告

    cout << "结论: 指向不可变，指向内容可变。\n\n";
}

void demoConstPointerToConst() {
    cout << "=== 4) const int* const p ===\n";

    int x = 7;
    const int* const p = &x;
    cout << "初始: x=" << x << ", *p=" << *p << "\n";

    // *p = 70; // error: 不能改内容
    // p = &x;  // error: 不能改指向

    x = 70; // 变量本身若不是 const，可以通过原变量修改
    cout << "x=70 后: *p=" << *p << "\n";
    cout << "结论: 通过 p 看，内容不可变，指向也不可变。\n\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    demoReferenceVsPointer();
    demoPointerToConst();
    demoConstPointer();
    demoConstPointerToConst();

    cout << "速记: 看 * 左右，离谁近就 const 谁。\n";
    cout << " - const int* p / int const* p : const 修饰 *p（内容）\n";
    cout << " - int* const p               : const 修饰 p（指针本身）\n";
    cout << " - const int* const p         : 两者都 const\n";

    return 0;
}
