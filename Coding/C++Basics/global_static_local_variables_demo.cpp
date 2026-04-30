/*
 * 全局变量 / 静态全局变量 / 局部变量 / 静态局部变量 Demo
 *
 * 对应知识点概要：
 * - 全局变量：定义在命名空间作用域（本文件顶层），默认外部链接；其它翻译单元用 extern 声明后可用。（external：形容词，外部的、外来的、外部可见的）
 * - 静态全局变量：文件顶层的 static，内部链接，仅本 .cpp 可见；与其它文件的同名 static 互不冲突。
 * - 局部变量：函数体内自动存储期，每次进入函数创建，退出销毁（栈帧上的“自动变量”）。
 * - 静态局部变量：仍在函数内可见，但只占一份存储，整个程序运行期间只初始化一次，生命周期持续到程序结束。
 *
 * 存储区域（典型实现上的直观说法，注释帮助记忆）：
 * - 静态存储区：全局变量、静态全局、静态局部。
 * - 栈：普通局部变量（自动变量）。
 *
 * 注意（教材常考点）：
 * - 不要在头文件里定义（分配存储的）全局变量；多个 .cpp include 同一头会导致“重复定义”链接错误。
 *   正确做法：头文件里 extern 声明，某一个 .cpp 里单独定义一次。
 *
 * 编译说明：
 * - 本 demo 依赖同目录下的 global_static_local_extern_partner.cpp（演示 extern 的另一翻译单元）。
 * - 示例：g++ -std=c++20 global_static_local_variables_demo.cpp global_static_local_extern_partner.cpp -o global_static_local_demo.exe
 *
 * Windows 终端中文乱码：源码为 UTF-8 时，cmd/PowerShell 常为 GBK。main 开头已调用 SetConsoleOutputCP(CP_UTF8)；
 * 若仍异常，可先执行 chcp 65001 再运行 exe。
 */

#include <iostream>
#ifdef _WIN32   // _WIN32 是 Windows 操作系统的一个宏定义，表示当前操作系统是 Windows。
#include <windows.h>   // windows.h 是 Windows 操作系统的一个头文件，包含了 Windows 操作系统的 API。
#endif

using namespace std;

// ---------- 文件作用域：全局 vs 静态全局 ----------

// 全局变量：外部链接（external linkage）。其它 .cpp 可写 extern int g_file_visible_global; 后使用（定义只能有一处）。
int g_file_visible_global = 1;

// 静态全局变量：内部链接（internal linkage）。仅本翻译单元内可见，名字不会与其它 .cpp 里的同名 static 冲突。
static int g_only_in_this_file = 2;

// 由 partner 文件定义，在此声明后即可在本文件使用（链接期解析）。
extern int g_defined_in_other_translation_unit;

// ---------- 函数：局部变量 vs 静态局部变量 ----------

void demo_local_vs_static_local(int call_id) {
    int automatic_local = 0;           // 每次调用重新创建，初值每次都可设为 0
    static int static_local_counter = 0;  // 只初始化一次，之后保持上次离开时的值

    automatic_local += call_id;
    static_local_counter += 1;

    cout << "  [call " << call_id << "] automatic_local = " << automatic_local
         << " | static_local_counter = " << static_local_counter << "\n";
    // 下面两行打印两个变量的地址（数值仅供直观对比存储区域，具体区间依赖平台）。
    //
    // cout << "..." << ... << "\n";
    // - cout：标准输出流；<< 表示“追加输出”，从左到右依次输出每一段。
    // - "automatic_local 地址: "：提示文字。
    // - static_cast<void*>(&automatic_local)
    //   - &automatic_local：取变量 automatic_local 的地址，类型为 int*（指向 int 的指针）。
    //   - static_cast<void*>(...)：把指针转成 void*，避免某些编译器对 char* / void* 与 << 的重载歧义，
    //                              并以“裸地址”的形式打印（通常是十六进制）。
    // - "\n"：换行。
    cout << "  automatic_local 地址: " << static_cast<void*>(&automatic_local) << "\n";
    // static_local_counter 与上一行同理：&static_local_counter 的类型是指向静态局部的指针，
    // 转成 void* 后打印；可与上一行的地址对比（两次调用里栈上局部地址往往重复同一栈槽附近，
    // 静态局部地址在各次调用中保持不变）。
    cout << "  static_local_counter 地址: " << static_cast<void*>(&static_local_counter) << "\n";
}

void print_scope_hints() {
    cout << "\n--- 地址粗看存储区域（栈上的局部 vs 静态局部，数值区间往往不同）---\n";
    int stack_probe = 42;
    static int static_probe = 99;
    cout << "  stack_probe (局部):       " << static_cast<void*>(&stack_probe) << "\n";
    cout << "  static_probe (静态局部):    " << static_cast<void*>(&static_probe) << "\n";
}

int main() {
#ifdef _WIN32   
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== 1) 全局变量（本文件定义）===\n";
    cout << "g_file_visible_global = " << g_file_visible_global << "\n";

    cout << "\n=== 2) 静态全局变量（仅本文件）===\n";
    cout << "g_only_in_this_file = " << g_only_in_this_file << "\n";

    cout << "\n=== 3) extern：变量在另一个 .cpp 里定义（见 global_static_local_extern_partner.cpp）===\n";
    cout << "g_defined_in_other_translation_unit = " << g_defined_in_other_translation_unit << "\n";

    cout << "\n=== 4) 局部变量 vs 静态局部变量（同一函数调用两次）===\n";
    demo_local_vs_static_local(1);
    demo_local_vs_static_local(2);

    print_scope_hints();

    cout << "\n=== 小结 ===\n";
    cout << "· 全局：整个程序一组定义；跨文件靠 extern 声明 + 单点定义。\n";
    cout << "· static 全局：锁在本文件，防止名字污染其它翻译单元。\n";
    cout << "· 局部 automatic：每次进函数重来；static 局部：记忆上次结果，生命周期贯穿程序。\n";

    return 0;
}
