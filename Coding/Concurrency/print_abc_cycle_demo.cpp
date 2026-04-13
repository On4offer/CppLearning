// ============================================================================
// 三线程循环打印 ABC
// - 线程 ta：只输出字符 'A'
// - 线程 tb：只输出字符 'B'
// - 线程 tc：只输出字符 'C'
// 连续输出 ABCABC…，共 ROUNDS 轮，即总共 3*ROUNDS 个字符（无空格、无换行，直至 main 最后 endl）。
//
// 核心逻辑：用全局整数 current_step 表示「下标」0 .. 3*ROUNDS-1，每打印一个字符后 current_step++。
//   current_step % 3 == 0 → 轮到 A；== 1 → 轮到 B；== 2 → 轮到 C。
// 这样三个线程通过「是否轮到自己」分工，顺序由步数严格决定，与 ta/tb/tc 的创建顺序无关。
// ============================================================================

#include <iostream>           // 标准输出：cout、endl；单字符也用 cout << 'A'
#include <thread>             // std::thread：创建、join
#include <mutex>              // std::mutex、std::unique_lock：互斥；wait 必须用 unique_lock
#include <condition_variable> // std::condition_variable：wait / notify_all

// using namespace std：把标准库名字导入当前作用域，可写 cout 而非 std::cout。
// 注意：仅在 .cpp 实现文件中使用较常见；若写在被多处 include 的头文件里，容易名字冲突。
using namespace std;

// ---------- 多线程共享的全局状态（读写都要在同一把锁保护下，或通过 wait 的原子配对）----------
mutex mtx;                     // 互斥量：同一时刻最多一个线程持有（unique_lock 管理）
condition_variable cv;         // 条件变量：线程在 wait 上阻塞；notify_all 唤醒后重新检查谓词

// current_step：下一个该打印的位置（从 0 开始）；每打印一个字符自增 1。
// 当 current_step 增长到 total（= 3*ROUNDS）时，表示全部打印完毕，三个线程都要结束循环。
int current_step = 0;

// ROUNDS：循环多少「轮」ABC（一轮 = ABC 三个字符）；总字符数 = 3 * ROUNDS
const int ROUNDS = 5;

/*
 * print_a：仅当 current_step % 3 == 0 且尚未打满时打印 'A'。
 *
 * wait(lk, 谓词) 中谓词为真的两种情况（用 || 连接）：
 *   1) current_step >= total —— 已全部打完，需要所有线程被唤醒以便退出 while（否则可能永远卡在 wait）；
 *   2) current_step < total && current_step % 3 == 0 —— 还没打完，且当前轮到 A。
 * [&]：lambda 按引用捕获外部的 current_step、total 等，体内读到的是「实时」的共享变量。
 */
void print_a() {
    const int total = 3 * ROUNDS;   // const 局部常量：本线程内总步数，避免魔法数字
    while (true) {                  // 循环直到 break；也可用 for，此处强调「直到条件满足才退出」
        unique_lock<mutex> lk(mtx); // 构造时 lock(mtx)；可与 cv.wait 配合（wait 会临时 unlock）

        // 线程执行到这里，会阻塞等待，直到【满足下面两个条件之一】才会醒来继续执行
        cv.wait(lk, [&] {
            return current_step >= total || (current_step < total && current_step % 3 == 0);
        });
        // 醒来后，继续执行下面的代码
    if (current_step >= total) {
        cout << "线程A结束" << endl;
        break;  // 结束：不再打印
    }
        cout << 'A';               // 单引号 'A' 类型为 char；与字符串 "A" 不同
        ++current_step;            // 等价 current_step += 1；步数前进，下一轮可能轮到 B
        lk.unlock();               // 手动提前释放锁；析构时 unique_lock 不会重复 unlock
        cv.notify_all();           // 唤醒其他可能在 wait 的线程（B/C 或同伴一起检查退出条件）
    }
}

// print_b / print_c：与 print_a 结构相同，仅「轮到谁」的条件分别为 %3==1 与 %3==2。
void print_b() {
    const int total = 3 * ROUNDS;
    while (true) {
        unique_lock<mutex> lk(mtx);
        cv.wait(lk, [&] {
            return current_step >= total || (current_step < total && current_step % 3 == 1);
        });
        if (current_step >= total) {
            cout << "线程B结束" << endl;
            break;
        }
        cout << 'B';
        ++current_step;
        lk.unlock();
        cv.notify_all();
    }
}

void print_c() {
    const int total = 3 * ROUNDS;
    while (true) {
        unique_lock<mutex> lk(mtx);
        cv.wait(lk, [&] {
            return current_step >= total || (current_step < total && current_step % 3 == 2);
        });
        if (current_step >= total) {
            cout << "线程C结束" << endl;
            break;
        }
        cout << 'C';
        ++current_step;
        lk.unlock();
        cv.notify_all();
    }
}

/*
 * main：程序入口；返回 int，0 表示正常结束。
 * thread ta(print_a)：创建线程 ta，子线程将执行函数 print_a（函数名退化为函数指针/可调用对象）。
 * join()：阻塞主线程直到对应子线程函数返回；三个都 join 后，再 cout endl 换行。
 */
int main() {
    cout << "N=" << ROUNDS << " 轮，期望输出 " << 3 * ROUNDS << " 个字符: ";
    // 创建三个线程，分别执行print_a、print_b、print_c
    // 三个线程会同时执行，但是会根据current_step的值来决定打印哪个字符
    // 当current_step的值为0时，打印A
    // 当current_step的值为1时，打印B
    // 当current_step的值为2时，打印C
    // 当current_step的值为3时，打印A
    // 当current_step的值为4时，打印B
    // 当current_step的值为5时，打印C
    // 当current_step的值为6时，打印A
    // 当current_step的值为7时，打印B

    // 结束条件：current_step的值为3*ROUNDS时，三个线程都结束
    thread ta(print_a);
    thread tb(print_b);
    thread tc(print_c);

    ta.join();
    tb.join();
    tc.join();

    cout << endl;
    return 0;
}
