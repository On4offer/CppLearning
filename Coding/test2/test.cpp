#include <iostream>
using namespace std;

int main() {
    int a = 10; // 栈内存变量
    int* p = &a; // p 是指针，存储 a 的内存地址（& 是取地址符）

    // 指针操作
    cout << "p 指向的内存地址：" << &p << endl; // 输出 a 的内存地址（如 0x7ffeefbff5ac）
    cout << endl;
    cout << "p 指向的对象：" << *p << endl; // 解引用，输出 a 的值（10）

    // 指针算术运算（Java 引用不可运算）
    p++; // 指针指向内存地址的下一个 int 位置（偏移 4 字节）

    // 空指针（指向空地址，不可解引用，否则崩溃）
    int* p2 = nullptr; // C++11 推荐用 nullptr，替代 NULL
    // *p2 = 20; // 报错，空指针解引用，程序崩溃

    // 堆内存指针
    cout << endl;
    int* p3 = new int(100); // 申请堆内存，存储 100
    cout << "p3 指向的对象：" << *p3 << endl; // 输出 100
    cout << endl;
    cout << "p3 指向的内存地址：" << p3 << endl; // 输出堆内存地址（如 0x61fe10）
    delete p3; // 释放堆内存
    p3 = nullptr; // 置空指针，避免悬空指针

    return 0;
}
