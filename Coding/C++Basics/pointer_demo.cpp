/*
 * 指针 Demo
 *
 * 覆盖知识点：
 * 1) 什么是指针（地址、解引用）
 * 2) 指针大小（和平台位数相关）
 * 3) 空指针 nullptr
 * 4) 通过指针修改变量
 * 5) 数组与指针（指针运算）
 * 6) 函数指针（回调）
 * 7) 危险示例（野指针、悬空指针、越界访问）
 */
#include <bits/stdc++.h>    // 引入头文件,包含所有标准库头文件，简化代码
using namespace std;

void changeByPointer(int* p) {    // 通过指针修改变量
    if (p == nullptr) return;    // 如果指针为空，则返回
    *p += 10;    // 通过指针修改变量的值
}

void swapByPointer(int* a, int* b) {    // 通过指针交换两个变量的值
    if (a == nullptr || b == nullptr) return;
    int t = *a; // t是a的值
    *a = *b; // a的值等于b的值
    *b = t; // b的值等于t的值
}

int add(int x, int y) { return x + y; }
int mul(int x, int y) { return x * y; }

// 运行操作，op是一个函数指针，指向一个函数，函数有两个int参数，返回一个int值
void runOperation(int x, int y, int (*op)(int, int), const string& name) {    // 运行操作，name是操作的名称
    // 输出操作的名称和结果，name(x, y) = op(x, y)
    cout << name << "(" << x << ", " << y << ") = " << op(x, y) << "\n";
}

void dangerExamples() {
    cout << "=== 7) 危险示例（默认不执行危险代码） ===\n";

    cout << "[危险1] 野指针：未初始化指针直接解引用（未定义行为）\n";
    cout << "  错误写法: int* wild; *wild = 10;\n";
    cout << "  安全写法: int value = 10; int* safe = &value;\n";

    cout << "[危险2] 悬空指针：delete 后继续使用（未定义行为）\n";
    int* heapPtr = new int(123);
    cout << "  分配后 *heapPtr = " << *heapPtr << "\n";
    delete heapPtr;
    heapPtr = nullptr;  // 释放后立即置空，避免悬空
    cout << "  安全写法: delete 后置 nullptr，再判空使用\n";
    if (heapPtr == nullptr) {
        cout << "  heapPtr 已置空，后续不会误用\n";
    }

    cout << "[危险3] 越界访问：访问数组边界外元素（未定义行为）\n";
    int arr[3] = {1, 2, 3};
    cout << "  错误写法: arr[3] 或 *(arr + 3)\n";
    cout << "  安全写法: 仅访问下标 [0, 2]，当前 arr[2] = " << arr[2] << "\n";

    cout << "[建议] 尽量使用智能指针和容器，减少手动内存管理风险。\n\n";
}

int main() {
    ios::sync_with_stdio(false); // 关闭同步，提高输入输出效率
    cin.tie(nullptr); // 解除 cin 与 cout 的绑定，提高输入输出效率

    cout << "=== 1) 指针基础 ===\n";
    int n = 42; // n的值为42
    int* p = &n;  // p 保存 n 的地址，p的值为n的地址
    cout << "n 的值: " << n << "\n";
    cout << "n 的地址: " << &n << "\n"; // &n是n的地址
    cout << "p 保存的地址: " << p << "\n"; // p的值为n的地址，相当于&n
    cout << "*p (解引用): " << *p << "\n\n"; // *p是解引用，*p的值为n的值，相当于n

    cout << "=== 2) 指针大小 ===\n";
    cout << "sizeof(int*): " << sizeof(int*) << " bytes\n"; // int*的大小为4字节
    cout << "sizeof(double*): " << sizeof(double*) << " bytes\n"; // double*的大小为8字节
    cout << "说明: 同一平台下，不同类型指针大小通常相同。\n\n";

    cout << "=== 3) 空指针 nullptr ===\n";
    int* nullPtr = nullptr; // nullPtr的值为nullptr
    cout << "nullPtr 是否为空: " << (nullPtr == nullptr ? "是" : "否") << "\n"; // nullPtr的值为nullptr，所以nullPtr == nullptr为true
    cout << "注意: 空指针不能解引用。\n\n";

    cout << "=== 4) 通过指针修改值 / 指针版交换 ===\n";
    int a = 5, b = 9;
    cout << "初始: a=" << a << ", b=" << b << "\n";
    changeByPointer(&a); // 通过指针修改变量a的值，a的值为5+10=15，传的是a的地址
    cout << "changeByPointer(&a) 后: a=" << a << "\n";
    swapByPointer(&a, &b); // 通过指针交换两个变量的值，a的值为9，b的值为15，传的是a和b的地址
    cout << "swapByPointer(&a, &b) 后: a=" << a << ", b=" << b << "\n\n";

    cout << "=== 5) 数组与指针 ===\n";
    int arr[5] = {10, 20, 30, 40, 50};  // 定义一个数组，数组的大小为5，数组的元素为10,20,30,40,50
    int* pa = arr;  // 等价于 &arr[0]，pa的值为数组arr的第一个元素的地址
    cout << "arr 首地址: " << arr << "\n";
    cout << "pa 首地址: " << pa << "\n"; // pa的值为数组arr的第一个元素的地址，相当于arr
    cout << "遍历数组（指针运算）: ";
    for (int i = 0; i < 5; ++i) {
        cout << *(pa + i) << (i == 4 ? '\n' : ' '); // *(pa + i)是解引用，*(pa + i)的值为数组arr的第i个元素的值，相当于arr[i]
    }
    cout << "\n";

    cout << "=== 6) 函数指针（回调） ===\n";
    int (*funcPtr)(int, int) = add; // funcPtr是一个函数指针，指向add函数，funcPtr的值为add函数的地址，add函数是前面定义的add函数
    cout << "funcPtr 指向 add: " << funcPtr(3, 4) << "\n"; // funcPtr(3, 4)是调用add函数，3和4是参数，funcPtr的值为add函数的地址，add函数是前面定义的add函数
    runOperation(6, 7, add, "add"); // runOperation(6, 7, add, "add")是运行add函数，6和7是参数，add是函数名，"add"是函数名
    runOperation(6, 7, mul, "mul"); // runOperation(6, 7, mul, "mul")是运行mul函数，6和7是参数，mul是函数名，"mul"是函数名
    cout << "\n";

    dangerExamples(); // 运行危险示例

    return 0;
}
