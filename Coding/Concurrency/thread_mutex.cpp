#include <iostream>  // 输入输出库，用于 cout 打印
#include <thread>    // 线程库，用于创建和控制线程
#include <vector>    // 向量容器，用于保存线程对象
#include <mutex>     // 互斥量库，用于线程间同步
using namespace std;

// 互斥量，保护共享变量 counter 的读写，防止多个线程同时修改导致错误。
// 这是一个全局变量，所有线程都能访问它。每次修改 counter 时，都必须先锁住 mtx，修改完成后再解锁。
mutex mtx;

// 共享变量，多个线程都会对它进行累加。
int counter = 0;

// 线程函数：由每个线程执行。
// id 表示线程编号，times 表示每个线程要增加多少次 counter。
void increment(int id, int times) {
    // 每个线程会循环 times 次，每次累加 counter。
    for (int i = 0; i < times; i++) {
        // 步骤1：尝试获取锁。
        // lock_guard 会在作用域结束时自动释放锁，避免忘记 unlock。
        // 这行代码会阻塞，直到拿到锁为止。
        lock_guard<mutex> lock(mtx);

        // 步骤2：进入临界区后，安全地修改 counter。
        // 现在只有这个线程能修改 counter，其他线程都等着。
        counter++;

        // 步骤3：输出当前线程和 counter 的最新值。
        // 这样可以看到线程交替执行的情况。
        cout << "Thread " << id << " incremented counter to " << counter << endl;

        // 步骤4：离开作用域时，lock_guard 自动释放锁。
        // 下一个线程可以继续。
    }
}

int main() {
    // 用来保存线程对象的向量。
    // 每个线程对象代表一个正在运行的线程。
    vector<thread> threads;

    // 创建 5 个线程，每个线程都执行 increment 函数。
    // 为什么是5个？为了演示多线程竞争。
    for (int i = 0; i < 5; i++) {
        // emplace_back 直接在向量末尾创建一个线程对象。
        // increment 是线程入口函数，i 是线程编号，10 是累加次数。
        // 这时线程就开始运行了！
        threads.emplace_back(increment, i, 10);
    }

    // 等待所有线程执行完毕。
    // join() 会阻塞当前线程（main 线程），直到目标线程结束。
    // 如果不 join，主线程可能会提前结束，导致程序崩溃。
    for (auto& t : threads) {
        t.join();
    }

    // 所有线程都结束后，输出最终结果。
    // 期望值是 5 * 10 = 50。
    cout << "Final counter value: " << counter << endl;

    return 0;
}
