// ============================================================================
// 演示：多线程对同一计数器做自增 —— 使用 std::atomic，无需 mutex 保护 counter 本身
// 5 个线程，每个线程执行 times 次 counter++，最终 counter 应为 5 * times（本例 50）。
//
// 说明：
// - atomic<int> 保证对 counter 的读/写/++ 是「原子操作」，不会出现「读改写」被打断导致丢更新。
// - cout 的输出仍可能被多个线程交错，屏幕上行的先后顺序不代表执行顺序；但 counter 的最终值正确。
// ============================================================================

#include <iostream>  // cout、endl
#include <thread>      // thread、join
#include <atomic>      // atomic<int>：原子整数
#include <vector>      // vector<thread>：存放多个线程对象

using namespace std;

// 全局原子计数器；初值 0。多线程同时 ++ 时由硬件/库保证原子性（默认 memory_order_seq_cst）
atomic<int> counter(0);

/*
 * increment：每个线程执行 times 次自增。
 * 参数 id：线程编号（0..4），仅用于打印；times：本线程循环次数。
 * counter++ 等价于原子上的 fetch_add(1)（返回值是旧值，这里未使用）。
 */
void increment(int id, int times) {
    for (int i = 0; i < times; i++) {
        counter++;
        cout << "Thread " << id << " incremented counter to " << counter << endl;
    }
}

/*
 * main：创建 5 个子线程，每个传入 (increment, 线程编号 i, 10)；join 等待全部结束后再读最终 counter。
 * emplace_back(increment, i, 10)：在 vector 尾部原地构造 thread，参数会转发给 thread 构造函数，
 *   等价于 thread(increment, i, 10)，即子线程将调用 increment(i, 10)。
 */
int main() {
    vector<thread> threads;

    // 创建 5 个线程
    cout << "创建 5 个线程" << endl;
    for (int i = 0; i < 5; i++) {
        threads.emplace_back(increment, i, 10);
        cout << "创建线程 " << i << "..." << endl;
    }
    cout << "5 个线程创建完成" << endl;

    // 等待所有线程完成：join() 阻塞直到对应子线程的 increment 返回
    cout << "等待所有线程完成" << endl;
    for (auto& t : threads) {
        t.join();
    }
    cout << "所有线程完成" << endl;

    cout << "Final counter value: " << counter << endl;

    return 0;
}
