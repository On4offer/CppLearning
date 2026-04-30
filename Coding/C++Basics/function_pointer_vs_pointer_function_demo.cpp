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

// 自定义类型，方便演示“返回指针”，作用是封装两个 int 变量，方便传递。
// 也可以用 class 代替 struct
// class Type {
// public:
//     int var1;
//     int var2;
// };
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
    return t;              // 返回指针，指向 Type 对象，也就是返回了 Type 对象的地址。
    // 这里返回的是 Type 对象的地址，而不是 Type 对象本身。
    // 所以 makeType 是“指针函数”，返回值是指针。
    // 也就是说，makeType 函数返回的是一个指向 Type 对象的指针。
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
    ios::sync_with_stdio(false);    // 关闭同步，提高输入输出效率
    cin.tie(nullptr);                // 解除 cin 和 cout 的绑定，提高输入输出效率

    cout << "===== 1) 指针函数：函数返回指针 =====\n";
    Type* p = makeType(5, 6);  // makeType 是函数，返回值类型是 Type*
    // makeType 函数返回的是一个指向 Type 对象的指针。
    // 所以 p 是一个指向 Type 对象的指针。
    // 所以 p->var1 和 p->var2 是 Type 对象的两个 int 变量。
    cout << "p->var1 = " << p->var1 << ", p->var2 = " << p->var2 << "\n";
    delete p;   // 释放 makeType 里 new 的内存
    p = nullptr;

    cout << "\n===== 2) 函数指针：指针变量指向函数 =====\n";
    // 语法拆解：
    // int        -> 指向的函数返回 int
    // (*fp)      -> fp 是“指针变量”（重点：* 和 fp 绑在一起）
    // (int, int) -> 指向的函数参数列表
    // 这里的 fp 是一个函数指针，指向一个返回 int 类型，参数列表为 (int, int) 的函数。
    // = 是赋值运算符，把 add 函数的地址赋值给 fp，也就是说，fp 指向 add 函数。
    int (*fp)(int, int) = add;  // fp 指向 add，fp也可以命名为别的名字，比如 func
    cout << "fp 指向 add, fp(10, 3) = " << fp(10, 3) << "\n";  // 调用 fp 指向的 add 函数，参数是 10 和 3

    fp = mul;  // 让 fp 改为指向 mul，也就是说，fp 指向 mul 函数
    cout << "fp 指向 mul, fp(10, 3) = " << fp(10, 3) << "\n";

    cout << "\n===== 3) 回调场景：把函数指针当参数传递 =====\n";

    // cout << ... << "\n";
    // - cout：标准输出对象，负责把内容打印到终端。
    // - "calculate(8, 4, add) = "：提示文本，先输出说明，再输出计算结果。
    // - << calculate(8, 4, add)：把函数调用结果接到输出流后面。
    //   * calculate：回调示例函数，签名是 calculate(int, int, int (*op)(int,int))。
    //   * 8, 4：传给 calculate 的前两个普通整型参数 x 和 y。
    //   * add：函数名在这里会退化为函数指针，作为第三个参数 op 传入。
    //     这表示“让 calculate 内部通过 op(x, y) 调用 add(8, 4)”。
    // - << "\n"：输出换行，方便下一条输出另起一行。
    cout << "calculate(8, 4, add) = " << calculate(8, 4, add) << "\n";
    cout << "calculate(8, 4, mul) = " << calculate(8, 4, mul) << "\n";

    cout << "\n===== 4) 易混淆点总结 =====\n";
    cout << "A. 指针函数：Type* func(...)  -> 本质是函数\n";
    cout << "B. 函数指针：Type (*fp)(...) -> 本质是指针变量\n";
    cout << "记忆口诀：看变量名附近，带 * 的是指针；在函数名左边的是返回类型。\n";

    return 0;
}

