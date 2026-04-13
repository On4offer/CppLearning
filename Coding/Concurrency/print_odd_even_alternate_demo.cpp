// ============================================================================
// 两线程交替打印奇数 / 偶数
// - 线程 t_odd（print_odd）：只打印奇数 1, 3, 5, …
// - 线程 t_even（print_even）：只打印偶数 2, 4, 6, …
// 控制台顺序为：1 2 3 4 … 一直到 MAX_VAL（含），然后两线程结束。
//
// 核心变量 next_val：表示「下一个该打印的整数」，从 1 开始；每打印一次执行 next_val++。
// 关键条件：
//   - 奇线程仅在 next_val 为奇数时打印（且未超过 MAX_VAL）；
//   - 偶线程仅在 next_val 为偶数时打印；
//   - 当 next_val 变为 MAX_VAL+1 时，两线程通过谓词中的 next_val > MAX_VAL 醒来并 break，避免死等。
// ============================================================================

#include <iostream>           // cout、endl
#include <thread>             // std::thread：创建子线程、join
#include <mutex>              // std::mutex、std::unique_lock（与 condition_variable::wait 配套）
#include <condition_variable> // wait：阻塞到谓词为真；notify_one：唤醒一个等待线程

// using namespace std：当前 .cpp 内可直接写 cout、thread 等，等价于 std::cout（单文件练习常用）
using namespace std;

// ---------- 全局同步对象与共享状态 ----------
mutex mtx;                     // 互斥量：保护对 next_val 的读、写，避免数据竞争
condition_variable cv;         // 条件变量：一方改完 next_val 后通知另一方「轮到你了」

// next_val：下一个该打印的数；初值为 1（第一个数必须是奇数线程打印）
int next_val = 1;

// MAX_VAL：打印上界（包含 MAX_VAL）；打印完 MAX_VAL 后 next_val 会再加 1，变为 MAX_VAL+1，用于结束
const int MAX_VAL = 100;

/*
 * print_odd：直到 next_val > MAX_VAL 时退出循环。
 *
 * wait(lk, 谓词) 的谓词为真，当且仅当：
 *   (1) next_val 为奇数 —— 轮到奇线程打印；或
 *   (2) next_val > MAX_VAL —— 工作已结束，需要醒来以便退出 while（否则可能永远阻塞在 wait）。
 * 若只用 (1)，打满后奇线程可能卡在 wait；因此必须加上 (2)。
 *
 * 醒来后若 next_val > MAX_VAL，则 break，不再打印。
 * notify_one()：只唤醒一个等在 cv 上的线程；本题另一线程只有一个偶线程在等待，用 notify_one 足够。
 */
void print_odd() {
    while (true) {                         // 无限循环，靠内部 break 退出
        unique_lock<mutex> lk(mtx);        // 构造：lock(mtx)；析构或 unlock 时释放

        // Lambda []：无捕获，谓词里直接读全局 next_val（也可用 [&] 显式按引用捕获）
        cv.wait(lk, [] { return next_val % 2 == 1 || next_val > MAX_VAL; });

        if (next_val > MAX_VAL) break;     // 已全部打印完毕（偶线程打完了最后一个偶数后 next_val 会超过上界）

        cout << next_val << " ";           // 此时 next_val 必为奇数且 <= MAX_VAL
        ++next_val;                        // 交给偶数：下一个应为偶数
        lk.unlock();                       // 先释锁，减少对方在 wait 返回后抢锁的等待
        cv.notify_one();                   // 唤醒偶线程（通常它在 wait 偶数条件）
    }
}

/*
 * print_even：与 print_odd 对称。
 * 谓词：next_val 为偶数时打印；或 next_val > MAX_VAL 时醒来退出。
 * 初始 next_val=1 为奇数，偶线程第一次会在 wait 上阻塞，直到奇线程打出 1 并把 next_val 变为 2。
 */
void print_even() {
    while (true) {
        unique_lock<mutex> lk(mtx);
        cv.wait(lk, [] { return next_val % 2 == 0 || next_val > MAX_VAL; });
        if (next_val > MAX_VAL) break;
        cout << next_val << " ";
        ++next_val;
        lk.unlock();
        cv.notify_one();
    }
}

/*
 * main：创建两个线程分别跑奇/偶逻辑；join 保证子线程结束后主线程再换行。
 * 线程启动顺序（先 t_odd 还是先 t_even）不影响最终 1..MAX_VAL 顺序，由 next_val 与谓词保证。
 */
int main() {
    cout << "交替打印 1 到" << MAX_VAL << ": ";
    thread t_odd(print_odd);               // thread 构造函数：传入可调用对象（函数指针 print_odd）
    thread t_even(print_even);

    t_odd.join();                          // 阻塞直到 print_odd 返回
    t_even.join();

    cout << endl;
    return 0;                              // int 返回值 0 常表示进程正常结束
}
