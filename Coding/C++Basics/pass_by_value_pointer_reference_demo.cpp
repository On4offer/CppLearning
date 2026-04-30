/*
 * 值传递 vs 指针传递 vs 引用传递 Demo
 *
 * Part 1 —— int：谁在改副本，谁在改原变量
 * Part 2 —— 大对象：引用传递的意义（避免拷贝、拷贝构造开销）
 *
 * 图片知识点对应：
 * - 值传递：形参是实参的拷贝；对大对象会触发拷贝构造，有额外开销。
 * - 指针传递：本质是“拷贝指针值”（仍是值传递语义），但通过同一地址访问原对象。
 * - 引用传递：形参是实参别名；常用于大参数以避免拷贝，并可就地修改。
 */

#include <iostream>

using namespace std;

// ---------- Part 1：int ----------

void changeByValue(int x) {
    cout << "[changeByValue] 形参 x 的地址: " << &x << ", 修改前 x = " << x << "\n";
    x += 10;
    cout << "[changeByValue] 形参 x 的地址: " << &x << ", 修改后 x = " << x << "\n";
}

void changeByPointer(int* p) {
    cout << "[changeByPointer] 指针 p 的值(即外部变量地址): " << p << "\n";
    cout << "[changeByPointer] *p 修改前 = " << *p << "\n";
    *p += 10;
    cout << "[changeByPointer] *p 修改后 = " << *p << "\n";
}

void changeByReference(int& x) {
    cout << "[changeByReference] 引用 x 的地址: " << &x << ", 修改前 x = " << x << "\n";
    x += 10;
    cout << "[changeByReference] 引用 x 的地址: " << &x << ", 修改后 x = " << x << "\n";
}

// ---------- Part 2：带拷贝计数的大对象（示意） ----------

struct BigThing {
    int id{};   // id是BigThing的成员变量，初始值为0，大括号{}是初始化列表，表示初始值为0
    BigThing() = default;   // BigThing() = default; 是构造函数的定义，表示默认构造函数，表示不进行任何操作
    explicit BigThing(int i) : id(i) {}   // explicit BigThing(int i) : id(i) {} 是构造函数的定义，表示初始化列表，表示初始值为i

    // 拷贝构造函数（Copy Constructor）：用已有 BigThing 初始化一个新 BigThing。
    // BigThing(...)           → 构造函数名与类名相同，表示这是 BigThing 的构造。
    // const BigThing& other   → 形参：对“源对象”的常量引用。
    //   - 传引用为避免再拷贝一次源对象（若写 BigThing other 会先拷进形参，逻辑别扭）。
    //   - const 表示承诺不修改源对象，符合“只读范本做拷贝”的语义。
    // : id(other.id)          → 构造函数初始化列表：用 other.id 初始化本对象的成员 id，
    //                           比在函数体内写 id = other.id 更高效（尤其对复杂成员）。
    BigThing(const BigThing& other) : id(other.id) {
        // ++g_copy_ctor_calls   → 静态计数器加一；本 demo 用它统计“发生了多少次拷贝构造”。
        ++g_copy_ctor_calls;
        // cout << ...           → 打印提示，便于在终端看到本次是第几次拷贝、拷贝后的 id 是多少。
        cout << "  >>> BigThing 拷贝构造 #" << g_copy_ctor_calls << " (id=" << id << ")\n";
    }

    // 拷贝赋值运算符（Copy Assignment）：形如 a = b，把右侧对象的值赋给已存在的左侧对象。
    // BigThing&               → 返回类型：惯例返回左操作数的引用，支持链式赋值 a=b=c。
    // operator=               → 运算符重载的特殊函数名，对应赋值符号 =。
    // (const BigThing&)       → 右侧来源对象：常量引用，避免无谓拷贝右操作数。
    // = delete                → 显式删除：禁止任何形式的赋值（编译器碰到 a=b 会报错）。
    // 本 demo 故意禁用赋值，避免出现“赋值计数”干扰我们只观察拷贝构造的行为。
    BigThing& operator=(const BigThing&) = delete;

    // static void reset_copy_count() { ... }
    // - static：属于 BigThing 类型本身，不属于某个具体对象；用 BigThing::reset_copy_count() 调用。
    // - void：无返回值。
    // - g_copy_ctor_calls = 0：把静态拷贝计数清零，便于每一段演示单独统计。
    static void reset_copy_count() { g_copy_ctor_calls = 0; }

    // static int copy_count() { return ... }
    // - static：同上，通过类型名调用 BigThing::copy_count()。
    // - int：返回当前累积的拷贝构造调用次数（由拷贝构造函数里 ++ 维护）。
    static int copy_count() { return g_copy_ctor_calls; }

private:
    // private：访问控制，其后成员仅能在 BigThing 内部（成员函数等）访问，外部不能直接读写该计数器，
    //          避免演示代码以外误改统计结果。
    // static：静态成员变量——整个类型共享一份存储，不属于某个具体的 BigThing 对象；
    //         所有 BigThing 实例发生的拷贝构造都累计到同一个数上。
    // inline（C++17）：允许在类内直接写出静态数据成员的定义并初始化，不必再在 .cpp 里单独定义一份，
    //                否则通常要在类外写 int BigThing::g_copy_ctor_calls = 0;
    // int：计数器用整型存储拷贝构造调用次数。
    // g_copy_ctor_calls：变量名（global copy constructor calls 的缩写风格），便于grep；也可叫 s_copy_count 等。
    // = 0：成员初始值为 0；程序开始时计数从零累计。
    static inline int g_copy_ctor_calls = 0;
};  // struct BigThing 定义结束；此后 bump_by_value 等为自由函数，不再是类成员。

// 值传递：进入函数前要拷贝整个 BigThing → 触发拷贝构造
void bump_by_value(BigThing x) {
    x.id += 1;   // x.id += 1; 表示 x 对象的 id 成员变量加 1
}

// 指针传递：只拷贝指针 sizeof(BigThing*)，不拷贝 BigThing 本体（演示：几乎无拷贝构造）
void bump_by_pointer(BigThing* p) {
    if (p) {
        p->id += 1;  // p->id += 1; 表示 p 指向的 BigThing 对象的 id 成员变量加 1
    }
}

// 引用传递：不拷贝对象，形参是别名 → 无拷贝构造（此处用来体现“避免拷贝”）
void bump_by_reference(BigThing& x) {
    x.id += 1;   // x.id += 1; 表示 x 对象的 id 成员变量加 1
}

// const 引用：只读访问大对象时常用，同样不触发拷贝构造
void print_by_const_reference(const BigThing& x) {
    cout << "  [const&] 只读访问 id=" << x.id << "\n";
}

void demo_part1_int() {
    int a = 100;
    cout << "初始: a = " << a << ", a 的地址 = " << &a << "\n\n";

    cout << "=== Part1 — 1) 值传递 ===\n";
    changeByValue(a);
    cout << "[main] 调用后 a = " << a << " (不变，函数里改的是副本)\n\n";

    cout << "=== Part1 — 2) 指针传递 ===\n";
    changeByPointer(&a);
    cout << "[main] 调用后 a = " << a << " (已变，通过 *p 改到原变量)\n\n";

    cout << "=== Part1 — 3) 引用传递 ===\n";
    changeByReference(a);
    cout << "[main] 调用后 a = " << a << " (已变，引用绑定原变量)\n\n";

    cout << "=== Part1 小结 ===\n";
    cout << "值：改副本 | 指针：传地址副本，*p 改原对象 | 引用：别名，直接改原对象\n\n";
}

void demo_part2_large_object() {
    cout << "=== Part2 — 大对象与「是否发生拷贝构造」===\n";
    cout << "(每出现一次拷贝构造打印一行，便于体会引用避免拷贝)\n\n";

    BigThing obj{42};
    BigThing::reset_copy_count();

    cout << "--- A) void bump_by_value(BigThing x)：按值传 → 至少一次拷贝 ---\n";
    bump_by_value(obj);
    cout << "[main] obj.id = " << obj.id << " (函数内改的是副本，外面不变)\n";   // obj.id 表示 obj 对象的 id 成员变量
    cout << "本轮累积拷贝构造次数: " << BigThing::copy_count() << "\n\n";

    BigThing::reset_copy_count();

    cout << "--- B) void bump_by_pointer(BigThing* p)：传指针 → 只拷贝指针 ---\n";
    bump_by_pointer(&obj);
    cout << "[main] obj.id = " << obj.id << "\n";   // obj.id 表示 obj 对象的 id 成员变量
    cout << "本轮累积拷贝构造次数: " << BigThing::copy_count() << "\n\n";

    cout << "--- C) void bump_by_reference(BigThing&)：引用 → 通常零拷贝 ---\n";
    bump_by_reference(obj);
    cout << "[main] obj.id = " << obj.id << "\n";
    cout << "本轮累积拷贝构造次数: " << BigThing::copy_count() << "\n\n";

    cout << "--- D) void print_by_const_reference(const BigThing&)：只读大参数常用写法 ---\n";
    print_by_const_reference(obj);
    cout << "本轮累积拷贝构造次数: " << BigThing::copy_count() << "\n\n";

    cout << "=== Part2 小结（对应教材）===\n";
    cout << "· 引用传递的常见目的：避免拷贝大对象、减少拷贝构造开销。\n";
    cout << "· 指针传递本质上是「指针值的值传递」，但通过同一地址间接访问原对象。\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    demo_part1_int();
    demo_part2_large_object();

    return 0;
}
