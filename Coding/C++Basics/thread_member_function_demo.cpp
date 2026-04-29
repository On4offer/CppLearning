/*
 * std::thread + 成员函数传参 Demo
 *
 * 覆盖知识点：
 * 1) 普通函数可直接传给 std::thread（函数名可隐式转函数指针）
 * 2) 非静态成员函数必须写成 &Class::func
 * 3) 非静态成员函数有 this，需额外传对象（常见写法传 &obj）
 * 4) static 成员函数无 this，用法接近普通函数
 * 5) 成员函数指针类型与普通函数指针类型不同
 *
 * 初学者阅读指南：
 * - 普通函数： thread(func, 参数...)
 * - 非静态成员函数： thread(&类名::函数, 对象地址, 参数...)
 * - 静态成员函数： thread(类名::静态函数, 参数...)
 */

#include <iostream>
#include <thread>
#include <string>
#include <functional>

using namespace std;  // 为了教学演示，省略 std::

// 全局函数：不属于任何类
void globalRun(const string& tag) {
    cout << "[globalRun] " << tag << "\n";  // 输出全局函数运行信息
}

// Task 类：演示成员函数、静态成员函数、成员变量
class Task {
public:
    // explicit: 防止隐式类型转换；std::move: 把临时值高效转移给成员变量
    explicit Task(string name) : name_(std::move(name)) {}

    // 非静态成员函数：调用时需要对象（因为有隐式 this 指针）
    void run(int times) {
        cout << "[Task::run] name=" << name_ << ", times=" << times
             << ", this=" << this << "\n";
    }

    // 静态成员函数：不依赖对象，没有 this
    static void staticRun(const string& tag) {
        cout << "[Task::staticRun] " << tag << "\n";
    }
private:
    string name_;  // 私有成员变量
};

// Worker 类：故意也叫 run，演示“同名成员函数必须写类名作用域”
class Worker {
public:
    void run() { cout << "[Worker::run]\n"; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== 1) 普通函数：可直接传函数名 ===\n";
    thread t1(globalRun, "from global function");  // thread 会把参数转发给 globalRun
    t1.join();                                     // 等待线程结束

    cout << "\n=== 2) 非静态成员函数：必须 &Class::func + 对象地址 ===\n";
    Task task("demo-task");
    // 关键语法：
    // &Task::run  -> 成员函数地址
    // &task       -> this 指针对应的对象地址
    // 3           -> run 的形参 times
    thread t2(&Task::run, &task, 3);  // 等价于在线程里调用 task.run(3)
    t2.join();

    cout << "\n=== 3) static 成员函数：无 this，用法接近普通函数 ===\n";
    thread t3(Task::staticRun, "from static member");
    t3.join();

    cout << "\n=== 4) 成员函数同名：必须用类作用域区分 ===\n";
    Worker worker;
    thread t4(&Worker::run, &worker);
    t4.join();

    cout << "\n=== 5) 成员函数指针类型演示 ===\n";
    // 成员函数指针语法：
    // 返回值类型 (类名::*指针名)(参数列表) = &类名::成员函数;
    void (Task::*memberFp)(int) = &Task::run;
    (task.*memberFp)(99);  // 通过对象调用成员函数指针

    cout << "\n=== 6) 常见错误（仅说明，不执行） ===\n";
    cout << "错误示例1：thread tx(Task::run, &task, 1);  // 缺少 &\n";
    cout << "错误示例2：thread ty(&Task::run, 1);        // 缺少对象 this\n";

    cout << "\n=== 7) 进阶：std::bind 方式 ===\n";
    // bind 先把“函数 + 对象 + 参数”绑定成一个可调用对象，再交给线程
    auto boundTaskRun = bind(&Task::run, &task, 7);
    thread t5(boundTaskRun);
    t5.join();

    cout << "\n=== 8) 进阶：lambda 方式（最常用） ===\n";
    // [&task] 是引用捕获：把外部变量 task 带进 lambda 使用
    // ()      是参数列表：这里为空，表示调用时不需要额外参数
    // {}      是函数体：线程执行的代码
    thread t6([&task]() { task.run(8); });
    t6.join();

    cout << "\n=== 9) 三种成员函数起线程写法对比 ===\n";
    cout << "A) thread(&Task::run, &task, n)   // 直接、清晰、推荐\n";
    cout << "B) thread(bind(&Task::run, &task, n)) // 适合复杂参数绑定\n";
    cout << "C) thread([&]{ task.run(n); })    // 灵活度最高，工程里常用\n";

    return 0;  // 程序正常结束
}

