/*
 * Lambda 在不同场景下的参数/捕获对照 Demo
 *
 * 目标：
 * 1) 看懂 [] 捕获和 () 形参的区别
 * 2) 看懂 std::thread / std::sort / std::async 的调用方式
 * 3) 纠正常见误区：thread 不是“只能无参 lambda”
 *
 * 初学者阅读指南：
 * - [] : 捕获列表。把“外部变量”带进 lambda 使用
 * - () : 参数列表。调用 lambda 时传进来的参数
 * - {} : 函数体。真正执行的代码
 *
 * 例子： [base](int x) { return base + x; }
 * - [base]  把外部变量 base 按值捕获
 * - (int x) 调用时传入参数 x
 * - { ... } 执行逻辑
 */

#include <algorithm>   // std::sort，
#include <future>      // std::async, std::future，异步
#include <functional>  // 可调用对象相关（本例主要是知识完整性）
#include <iostream>    // std::cout，输出
#include <thread>      // std::thread，线程
#include <vector>      // std::vector，向量

using namespace std;   // 教学示例中简化写法，避免每次都写 std::

// 工具函数：打印分隔标题，方便观察每一段输出
void printLine(const string& title) {
    cout << "\n========== " << title << " ==========\n";  // 打印形如 ===== 标题 =====
}

int main() {
    ios::sync_with_stdio(false);  // 关闭 iostream 与 stdio 同步，提升输出性能
    cin.tie(nullptr);             // 解除 cin/cout 绑定（本例主要是习惯性写法）

    int base = 10;  // 外部变量：后续演示“捕获”时会用到

    printLine("1) [] vs () 基础");                          // 第1段：先建立最小认知
    // 下面这行是一个完整 lambda 定义，逐段解释如下：
    // auto        : 自动类型推导。让编译器推导 f 的真实类型（lambda 的类型很长，通常不手写）。
    // f           : 变量名。f 保存这个 lambda，可像函数一样调用：f(5)。
    // =           : 赋值。把右侧创建出来的 lambda 对象赋给变量 f。
    // [base]      : 捕获列表。把外部变量 base 按“值捕获”带进 lambda 内部使用。
    //               值捕获含义：lambda 内部拿到的是 base 的一个副本，不会改到外部原变量。
    // (int x)     : 参数列表。调用 f(...) 时传入的参数，x 是形参，类型是 int。
    // { ... }     : 函数体。lambda 被调用时真正执行的代码。
    // return ...  : 返回语句。把计算结果返回给调用方。
    // base + x    : 业务逻辑。用捕获进来的 base 和传入参数 x 相加。
    // ;           : 语句结束符。表示这一条定义语句结束。
    auto f = [base](int x) { return base + x; };
    // 注意：f 虽然是“变量”，但它保存的是一个 lambda 对象（可调用对象）。
    // 因为 lambda 类型内部实现了 operator()，所以可以写 f(5)。
    // 也可以理解为：f(5) 等价于 f.operator()(5)。
    cout << "f(5) = " << f(5) << "\n";                      // 调用时给 x 传 5，结果 10+5
    cout << "解释: [base] 是捕获外部变量, (int x) 是调用时传入参数\n";  // 明确二者分工

    printLine("2) std::thread + lambda(捕获方式)");  // 第2段：线程里常见写法（捕获）
    // 下面这行同样逐段拆解：
    // thread      : 类型名，表示“线程对象类型”（std::thread，这里用了 using namespace std）。
    // t1          : 线程对象变量名。之后可用 t1.join() 等待线程结束。
    // ( ... )     : 构造参数。告诉线程“启动后执行什么任务”。
    // [&base]() { ... } : 传给线程的 lambda 任务（可调用对象）。
    //   [&base]   : 捕获列表。把外部变量 base 以“引用”方式带入 lambda。
    //               引用捕获意味着 lambda 内部改 base，会影响外部原变量。
    //   ()        : 参数列表。这里为空，表示这个 lambda 调用时不需要额外参数。
    //   { ... }   : 函数体。线程真正执行的代码块。
    // ;           : 语句结束。
    thread t1([&base]() {                            // 引用捕获 base，线程函数无形参
        base += 5;                                   // 在线程中修改外部变量
        cout << "thread t1: base 改为 " << base << "\n";  // 打印修改后的值
    });
    t1.join();  // 等待 t1 结束，避免主线程提前退出

    printLine("3) std::thread + lambda(形参方式, 也可以)");  // 第3段：thread 也能给 lambda 传参
    // 下面这行逐段拆解：
    // thread      : 线程类型（std::thread）。
    // t2          : 线程对象变量名。
    // ( ... )     : 构造线程时传入的“任务 + 任务参数”。
    // [](int a, int b) { ... } : 一个带两个形参的 lambda 任务。
    //   []        : 捕获列表为空。表示这个 lambda 不使用外部变量。
    //   (int a, int b) : 参数列表。a、b 是线程启动时传入给 lambda 的参数。
    //   { ... }   : 线程要执行的代码体。
    // , 3, 4      : 任务参数。std::thread 会把 3、4 转发给 lambda 的 a、b。
    //               对应关系是 a=3, b=4。
    // ;           : 语句结束。
    thread t2([](int a, int b) {                             // lambda 有两个形参 a,b
        cout << "thread t2: a + b = " << (a + b) << "\n";   // 在线程函数体中使用形参
    }, 3, 4);                                                // 3、4 由 thread 转发给 lambda
    t2.join();                                                // 等待 t2 完成

    printLine("4) std::thread + 先定义可调用对象再传入");      // 第4段：先存再传的常见写法
    auto worker = [](int a, int b) {                         // worker 是 lambda（可调用对象）
        cout << "worker: " << a * b << "\n";                 // 计算并输出乘积
    };
    thread t3(worker, 6, 7);                                 // thread 给 worker 传入 6 和 7
    t3.join();                                                // 等待 t3 完成

    printLine("5) std::sort 会主动传参给比较器");               // 第5段：算法场景，lambda 形参非常常见
    // 下面两行逐段拆解：
    // vector<int> nums = {5, 1, 4, 2, 3};
    // vector<int> : 类型，表示“int 动态数组”。
    // nums        : 变量名，保存这个数组容器。
    // = { ... }   : 列表初始化，把 5/1/4/2/3 放入容器。
    //
    // sort(nums.begin(), nums.end(), [](int lhs, int rhs) { return lhs < rhs; });
    // sort(...)   : 标准库排序算法（来自 <algorithm>）。
    // nums.begin(): 起始迭代器，指向第一个元素。
    // nums.end()  : 结束迭代器，指向“最后一个元素的下一个位置”。
    // 前两个参数合起来表示排序区间：[begin, end)。
    // 第三个参数是“比较器”lambda，sort 会反复调用它来决定顺序。
    // [](int lhs, int rhs)
    //   []        : 捕获列表为空，比较器不依赖外部变量。
    //   (int lhs, int rhs) : 比较器形参。sort 每次拿两个元素传进来比较。
    // return lhs < rhs;
    //   返回 true  : 说明 lhs 应排在 rhs 前面（升序）。
    //   返回 false : 说明 lhs 不应排在 rhs 前面。
    vector<int> nums = {5, 1, 4, 2, 3};                      // 待排序数组
    sort(nums.begin(), nums.end(), [](int lhs, int rhs) {    // sort 主动给比较器传 lhs/rhs
        return lhs < rhs;                                     // 升序规则
    });
    cout << "sort 后: ";                                      // 输出排序结果
    for (int n : nums) cout << n << ' ';                     // 范围 for 遍历
    cout << "\n";                                             // 换行

    printLine("6) std::async 也会转发参数给 lambda");          // 第6段：异步任务也支持参数转发
    // 下面这句逐段拆解：
    // future<int> fut = async(launch::async, [](int x, int y) { return x + y; }, 100, 23);
    //
    // future<int> : 类型。表示“将来会拿到一个 int 结果”的结果句柄。
    // fut         : 变量名。用于保存异步任务的结果句柄。
    // =           : 赋值。把 async 返回的 future 赋给 fut。
    //
    // async(...)  : 启动异步任务。它会执行你给的可调用对象（这里是 lambda）。
    //
    // launch::async : 启动策略，表示“立即在新线程上异步执行”。
    //                 （不是延迟到 fut.get() 才执行）
    //
    // [](int x, int y) { ... } : 异步任务函数（lambda）。
    //   []        : 捕获列表为空，不依赖外部变量。
    //   (int x, int y) : 形参列表，接收 async 传入的参数。
    //   { return x + y; } : 任务逻辑，返回 x+y，返回类型是 int。
    //
    // , 100, 23   : 传给 lambda 的实参。对应关系：x=100, y=23。
    //
    // 这句执行完后，真正结果还在“未来”里；要用 fut.get() 才取到结果。
    // fut.get() 会在必要时阻塞等待任务完成，然后返回 int 结果。
    future<int> fut = async(launch::async, [](int x, int y) { // 指定异步策略为 launch::async
        return x + y;                                          // 异步计算 x+y
    }, 100, 23);                                               // 100、23 转发给 lambda 形参
    cout << "async 结果: " << fut.get() << "\n";               // get 会等待结果并取值

    printLine("7) 常见误区总结");  // 第7段：快速复习，避免概念混淆
    cout << "误区A: thread 的 lambda 只能写 []() {...}  (错误)\n";
    cout << "更正: thread 也可以给 lambda 传参数, 如 thread(fn, 3, 4)\n";
    cout << "误区B: [] 和 () 是一回事 (错误)\n";  // 错误观念B
    cout << "更正: [] 捕获外部变量, () 接收调用参数\n";

    printLine("8) 一眼记忆");
    cout << "[] 抓外部变量, () 收调用参数, {} 写执行逻辑\n";
    cout << "线程中常见: thread([&]{ ... }); 或 thread(fn, arg1, arg2);\n";

    return 0;  // 程序正常结束
}

