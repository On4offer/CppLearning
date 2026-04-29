/*
 * 函数指针 vs 指针函数 Demo
 *
 * 目标：
 * 1) 彻底分清“指针函数”和“函数指针”
 * 2) 看懂两种写法在语法上的关键差异
 * 3) 学会在回调场景里使用函数指针
 *
 * 一句话先记住：
 * - 指针函数：它是“函数”，只是返回值是指针
 * - 函数指针：它是“指针变量”，只是这个指针指向函数
 */

#include <iostream>

using namespace std;

// 自定义类型，方便演示“返回指针”
struct Type {
    int var1;
    int var2;
};

// =========================
// 一、指针函数（Pointer Function）
// =========================
// 这是“函数”：函数名是 makeType
// 返回类型是 Type*，所以它是“指针函数”（返回指针的函数）
Type* makeType(int a, int b) {
    Type* t = new Type();  // 在堆上申请对象，返回对象地址
    t->var1 = a;
    t->var2 = b;
    return t;              // 返回指针
}

// =========================
// 二、普通函数（后面给函数指针指向）
// =========================
int add(int x, int y) {
    return x + y;
}

int mul(int x, int y) {
    return x * y;
}

// 回调示例：参数 op 是“函数指针”
int calculate(int x, int y, int (*op)(int, int)) {
    // op 指向某个函数，调用 op(x, y) 等价于调用被指向函数
    return op(x, y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "===== 1) 指针函数：函数返回指针 =====\n";
    Type* p = makeType(5, 6);  // makeType 是函数，返回值类型是 Type*
    cout << "p->var1 = " << p->var1 << ", p->var2 = " << p->var2 << "\n";
    delete p;   // 释放 makeType 里 new 的内存
    p = nullptr;

    cout << "\n===== 2) 函数指针：指针变量指向函数 =====\n";
    // 语法拆解：
    // int        -> 指向的函数返回 int
    // (*fp)      -> fp 是“指针变量”（重点：* 和 fp 绑在一起）
    // (int, int) -> 指向的函数参数列表
    int (*fp)(int, int) = add;  // fp 指向 add
    cout << "fp 指向 add, fp(10, 3) = " << fp(10, 3) << "\n";

    fp = mul;  // 让 fp 改为指向 mul
    cout << "fp 指向 mul, fp(10, 3) = " << fp(10, 3) << "\n";

    cout << "\n===== 3) 回调场景：把函数指针当参数传递 =====\n";
    cout << "calculate(8, 4, add) = " << calculate(8, 4, add) << "\n";
    cout << "calculate(8, 4, mul) = " << calculate(8, 4, mul) << "\n";

    cout << "\n===== 4) 易混淆点总结 =====\n";
    cout << "A. 指针函数：Type* func(...)  -> 本质是函数\n";
    cout << "B. 函数指针：Type (*fp)(...) -> 本质是指针变量\n";
    cout << "记忆口诀：看变量名附近，带 * 的是指针；在函数名左边的是返回类型。\n";

    return 0;
}

