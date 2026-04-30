/*
 * C++：struct 与 class 的两点默认差异
 * 1) 成员默认访问：struct 为 public；class 为 private。
 * 2) 继承默认方式：struct 为 public；class 为 private。
 * 保留 struct 主要为兼容 C。
 */

#include <iostream>

using namespace std;

// struct StructDefaultAccess { ... };
// - struct：与 class 一样可以定义类型；本 demo 用它演示「默认访问权限」——未写访问标签时成员默认为 public。
// - StructDefaultAccess：类型名；用它声明变量（如 StructDefaultAccess sa）。
struct StructDefaultAccess {
    // int x{}：成员变量；{} 表示默认初始化（此处等价于 x 初始为 0）。
    // 这是 C++11 新增的类内就地初始化。而且用的是 {} 空大括号初始化 = 强制零初始化 = 初始化为0
    //           struct 成员默认对外可见（public），外部可直接读写 x。
    int x{};
    // void f() const：成员函数；const 表示该函数承诺不修改 *this 的成员状态（此处不在函数体内改 x）。
    //                  f 同样是 struct 的 public 成员，外部对象可直接调用。
    void f() const {
        // cout << ...：通过全局引入的 cout（见 using namespace std）打印调试信息。
        cout << "StructDefaultAccess::f(), x=" << x << "\n";
    }
};  // struct 定义末尾必须有分号。

// class ClassWithPublic { ... };
// - class：与 struct 一样可以定义类型；但若未写访问标签，成员默认为 private。
// - 本段在类内写了 public:，因此其后的成员对外可见，方便与「显式公开」的 class 写法对照。
class ClassWithPublic {
public:
    // public:：访问说明符；从这里直到下一个 private:/protected: 之前，成员均为 public。
    // int y{}：成员变量；{} 默认初始化为 0；因位于 public 段，外部可读写 y。
    int y{};
    // void g() const：成员函数；const 表示不通过该函数修改对象状态（此处未在 g 内改 y）。
    void g() const {
        cout << "ClassWithPublic::g(), y=" << y << "\n";
    }
};  // class 定义末尾同样需要分号。

struct Base {
    int x{};
};

// struct DerivedFromStruct : Base { ... };
// - DerivedFromStruct：派生类类型名。
// - : Base：继承语法；写作 struct D : Base 且未写 public/private/protected 时，
//          struct 默认采用 public 继承 → Base 的 public 成员在派生类对外接口上保持可访问（见 main 里 d1.x）。
// - void bump()：派生类成员函数；函数体内可使用从 Base 继承来的成员 x。
// - ++x：自增继承得到的 x（语义上访问的是 Base 子对象里的那个 x）。
struct DerivedFromStruct : Base {
    void bump() {
        ++x;
    }
};  // 定义末尾分号。

// class DerivedFromClass : Base { ... };
// - class D : Base 且继承方式未写 public/private/protected 时，默认为 private 继承：
//   Base 中的 public 成员在 DerivedFromClass 对象上会变为派生类的「私有继承下来的成员」，
//   外部调用者通常不能像 d2.x 那样直接点名访问（教材对照）；成员函数体内仍可访问 x。
// - public:：下面列出的 bump / peek 是派生类自己的对外接口（不是继承访问级别），便于 main 调用。
class DerivedFromClass : Base {
public:
    // bump：修改继承而来的 x（++x）。
    void bump() {
        ++x;
    }
    // peek：读出当前 x；演示「私有继承」下仍可通过成员函数间接读到基类数据。
    int peek() const {
        return x;
    }
};  // 定义末尾分号。

int main() {
    cout << "=== 1) 默认成员访问 ===\n";
    StructDefaultAccess sa; // 声明一个 StructDefaultAccess 类型的对象 sa
    sa.x = 10; // 访问 StructDefaultAccess 类型的对象 sa 的成员变量 x，并赋值为 10
    sa.f(); // 访问 StructDefaultAccess 类型的对象 sa 的成员函数 f

    // class 未写访问说明符时成员默认为 private；显式 public 后才可与下面用法一致。
    ClassWithPublic cb;
    cb.y = 20;
    cb.g();

    cout << "\n=== 2) 默认继承 ===\n";
    DerivedFromStruct d1;
    d1.x = 10;
    d1.bump();
    cout << "DerivedFromStruct d1.x=" << d1.x << "  （默认 public 继承，类外可访问 Base::x）\n";

    DerivedFromClass d2;
    d2.bump();
    cout << "DerivedFromClass peek=" << d2.peek()
         << "  （默认 private 继承：类外不宜直接写 d2.x，成员函数内仍可访问）\n";

    cout << "\n=== 小结 ===\n";
    cout << "struct/class 能力相同；差别仅在默认访问与默认继承。\n";

    return 0;
}
