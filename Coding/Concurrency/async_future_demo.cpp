// ============================================================================
// 演示：std::async + std::future —— 在另一线程异步执行任务，主线程通过 future 取结果或等待完成
// - future<int>：异步函数有返回值（calculate）
// - future<void>：异步过程无返回值，只关心何时结束（task）
// launch::async：要求尽量在新线程中执行（与 launch::deferred 惰性求值不同）
// ============================================================================

#include <iostream>  // cout、endl
#include <future>    // std::async、std::future、std::launch
#include <thread>    // this_thread::sleep_for（亦可配合 <chrono>）
#include <chrono>    // chrono::seconds 等时间单位（建议显式包含，避免依赖间接包含）

using namespace std;

/*
 * calculate：在异步线程里被调用；模拟耗时计算后返回两数之和。
 * this_thread::sleep_for：阻塞「当前」线程（这里是执行 calculate 的那条异步线程），单位 chrono::seconds(2)。
 */
int calculate(int a, int b) {
    this_thread::sleep_for(chrono::seconds(2));
    return a + b;
}

/*
 * task：无返回值（void），仅模拟耗时后打印一行。
 */
void task() {
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Task completed" << endl;
}

/*
 * main：
 * 1) async(launch::async, calculate, 5, 3)
 *    - 启动异步执行 calculate(5,3)，返回 future<int>，稍后用 .get() 取结果（会阻塞直到算完）。
 * 2) async(launch::async, task)
 *    - 启动异步执行 task()，返回 future<void>；.wait() 只等待结束，不取返回值。
 *
 * future::get()：阻塞直到共享状态就绪，并取出返回值（只能 get 一次，对 future<int>）。
 * future::wait()：阻塞直到就绪；对 void 常用 wait() 同步「做完再继续」。
 */
int main() {
    // 异步计算：子线程跑 calculate；主线程可先干别的事，再 result.get() 取结果
    // async是一个函数模板，用于创建一个异步任务，返回一个future对象，future对象可以用来获取异步任务的结果
    // launch::async表示异步任务在新线程中执行，calculate是异步任务的函数，5和3是异步任务的参数
    // result.get()表示获取异步任务的结果，会阻塞直到异步任务完成，然后返回结果
    future<int> result = async(launch::async, calculate, 5, 3);
    cout << "Calculating..." << endl;
    cout << "Result: " << result.get() << endl;  // 此处阻塞至 calculate 返回（约 2 秒后打印 8）

    // 异步任务：无返回值，用 future<void>
    future<void> task_result = async(launch::async, task);
    cout << "Task started..." << endl;
    task_result.wait();  // 等待 task 跑完（约再等 1 秒）；若用 .get() 对 void 也可以

    return 0;
}