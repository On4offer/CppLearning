#include <iostream>
using namespace std;

void func(int* p) {
    cout << "func(int*) called" << endl;
}

void func(int i) {
    cout << "func(int) called" << endl;
}

int main() {
    // nullptr 是指针类型
    // p是一个指针变量，nullptr是一个空指针常量，表示指针不指向任何有效的内存地址
    // p的值被初始化为 nullptr，表示它不指向任何有效的内存地址
    int* p = nullptr;
    cout << "定义了一个指针 p，并将其初始化为 nullptr" << endl;
    // p = 0 代表指针 p 被赋值为整数 0，这在 C++ 中被解释为一个空指针常量，表示指针不指向任何有效的内存地址
    // 地址 0 是系统保留地址，代表 “无指向”，也就是空指针
    cout << "传入p: " << p << endl;
    func(p);      // 调用 func(int*)
    cout << "传入nullptr: " << nullptr << endl;

    // nullptr 它不是 int，也不是 int*。它是 C++11 专门的空指针常量：std::nullptr_t
    // 标准库给它专门写了输出规则：
    // 打印 nullptr 时，直接输出字符串 "nullptr"
    func(nullptr); // 调用 func(int*)
    cout << "传入0: " << 0 << endl;
    func(0);       // 调用 func(int)
    
    // NULL 是宏，可能被定义为 0
    // func(NULL);    // 可能调用 func(int)，取决于编译器
    
    return 0;
}