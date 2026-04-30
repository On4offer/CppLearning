/*
 * struct / union / class 对照 Demo（对应教材图解）
 *
 * 1) union：成员共享同一块内存，同一时刻通常只“语义上”使用其中一个；
 *    sizeof(union) 大致取最大成员并按对齐要求补齐。
 * 2) struct：成员各占存储（可能有对齐填充）；
 *    sizeof(struct) 为布局总跨度（含尾部填充）。
 * 3) class vs struct：默认成员访问与默认继承方式不同（见下半段）。
 * 4) 模板：写 template<class T> 或 template<typename T>；不能写 template<struct T>。
 *
 * 注意：下列 sizeof 数值依赖平台对齐规则；一般为常见 x86_64 Windows/Linux（GCC/Clang）。
 */

#include <cstddef>
#include <iostream>

using namespace std;

// ---------- 1) union：共享存储 ----------

union UnionCharAndInt {
    char c[10];
    int i;
};

union UnionCharAndDouble {
    char c[10];
    double d;
};

// ---------- 2) struct：成员顺序影响对齐填充 ----------

struct StructCharThenDouble {
    char c;
    double d;
};

struct StructCharDoubleChar {
    char c;
    double d;
    char cc;
};

// ---------- 3) 默认继承：struct : Base vs class : Base ----------

class DemoBaseForInherit {
public:
    void funA() const {
        cout << "DemoBaseForInherit::funA()\n";
    }
};

struct StructPublicInherit : DemoBaseForInherit {};

class ClassPrivateInherit : DemoBaseForInherit {};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== A) union vs struct：sizeof（含对齐填充）===\n";
    cout << "sizeof(UnionCharAndInt)      = " << sizeof(UnionCharAndInt)
         << "   // max(10,int) 并按对齐补齐\n";
    cout << "sizeof(UnionCharAndDouble)   = " << sizeof(UnionCharAndDouble)
         << "   // max(10,double) 并按对齐补齐\n";
    cout << "sizeof(StructCharThenDouble) = " << sizeof(StructCharThenDouble)
         << "   // char + 填充 + double\n";
    cout << "offsetof(StructCharThenDouble, d) = " << offsetof(StructCharThenDouble, d)
         << "   // 可见 char 后曾对齐全到 double\n";
    cout << "sizeof(StructCharDoubleChar) = " << sizeof(StructCharDoubleChar)
         << "   // 成员顺序改变尾部填充\n";

    cout << "\n=== B) class vs struct：默认继承对外接口的影响 ===\n";
    StructPublicInherit ex2;
    ex2.funA();

    [[maybe_unused]] ClassPrivateInherit ex3;
    cout << "ClassPrivateInherit：默认 private 继承；若在 main 写 ex3.funA() 将无法编译。\n";
    // ex3.funA();

    cout << "\n=== C) 模板形参关键字 ===\n";
    cout << "正确：template<class T> 或 template<typename T>\n";
    cout << "错误：template<struct T>（struct 不能用作此处关键字）。\n";

    return 0;
}
