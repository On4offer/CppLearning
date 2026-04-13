// ============================================================================
// LeetCode 1114. 按序执行（Print in Order）
// 三个线程分别跑 first / second / third，控制台输出必须是：first second third（顺序固定，只跑一轮）
// 核心：共享变量 stage 记录进度；后两个线程在 condition_variable 上 wait，直到前置步骤把 stage 改到位。
// ============================================================================

#include <iostream>           // cin / cout
#include <thread>             // std::thread：创建系统线程
#include <mutex>              // mutex、lock_guard、unique_lock：互斥锁
#include <condition_variable> // condition_variable：wait / notify，配合 unique_lock 使用

// using namespace std：把 std 里的名字导入当前作用域，下面可写 cout 而不用 std::cout
using namespace std;

// ---------- 全局同步对象（所有线程都能看到，故用来传递「进度」）----------
mutex mtx;                    // 互斥量：同一时刻最多一个线程能锁住它，保护 stage 的读写
condition_variable cv;        // 条件变量：线程可在上面阻塞（wait），其他线程 notify 可唤醒
// stage：整型「阶段」标志，和题目语义对应
//   0 — first 还没执行完（second、third 必须等）
//   1 — first 已执行完，second 可以执行
//   2 — second 已执行完，third 可以执行
int stage = 0;

/*
 * first：最先应执行的逻辑（题目里对应「先打印 first」）
 * void first() — void 表示无返回值；first 是函数名；() 内无参数。
 */
void first() {
    // 先完成「业务」：输出单词（无锁也可以，因为此时 second/third 还在 wait，不会和这里抢 console 顺序问题——
    // 真正要保证的是 stage 的发布顺序；打印放在锁外略减少持锁时间，本题数据量小，放锁内也可以）
    cout << "first " << endl;
    {
        // 花括号限定一块作用域：只为让 lock_guard 的析构（出块时自动解锁）落在一个明确范围
        lock_guard<mutex> lk(mtx);  // lock_guard<mutex>：模板类，lk 构造时对 mtx lock；析构时 unlock（RAII）
        stage = 1;                  // 在持锁下写共享变量，保证 second 里看到的 stage>=1 不会读到「半个更新」
    }                               // 离开块 → lk 析构 → mtx 解锁
    cv.notify_all();                // 唤醒所有在 cv 上 wait 的线程；second/third 会去检查自己的谓词是否满足
}

/*
 * second：必须在 first 把 stage 设为 1 之后才能往下执行
 *
 * 为什么用 unique_lock 而不是 lock_guard？
 *   condition_variable::wait 会「在阻塞时临时解锁 mtx」，醒来时再重新加锁；
 *   只有 unique_lock 支持这种配合，lock_guard 不能用于 cv.wait。
 *
 * wait 的第二个参数是谓词（predicate）：返回 true 时 wait 才返回；否则继续等。
 *   这里 [] { return stage >= 1; } 是无名 lambda：捕获列表 [] 为空，不捕获外部变量；
 *   stage 是全局变量，lambda 体内直接读全局名即可（也可用 [&] 显式按引用捕获，效果类似本题场景）。
 */
void second() {
    {
        // unique_lock<mutex> lk(mtx)：构造时锁住 mtx；可手动 unlock，且能与 cv.wait 配合
        unique_lock<mutex> lk(mtx);

        // wait(lk, predicate)：
        //   若 stage>=1 已为真，直接通过；
        //   否则阻塞当前线程，并把 lk 暂时解开（让别的线程能进临界区改 stage），
        //   被 notify 后醒来会再次检查 predicate，直到为真才返回（避免「虚假唤醒」）。
        //   说明：虚假唤醒靠「反复检查谓词」解决，与写 >= 还是 == 无关。这里用 >=1 表示语义「first 已完成（进度至少到 1）」；
        //   本题只有 0→1→2，谓词写成 stage==1 也可以，效果相同。
        cv.wait(lk, [] { return stage >= 1; });

        cout << "second " << endl;   // 谓词为真后，说明 first 已完成，可以打印 second
        stage = 2;           // 仍在本块内、仍持锁，安全地把阶段推进到 2，供 third 使用
    }                        // 右花括号：lk 析构 → 自动 unlock(mtx)。先释锁再 notify 是常见写法，减少 third 被唤醒后抢锁时的不必要的阻塞
    cv.notify_all();         // 通知：third 可能在 wait(stage>=2)，这里 stage 已是 2
}

/*
 * third：必须在 second 把 stage 设为 2 之后才能执行
 * 本函数在持锁状态下完成 wait 与打印；打印完函数结束，lk 析构解锁。无需再 notify（没有第四段）。
 */
void third() {
    unique_lock<mutex> lk(mtx);              // 从构造起一直持锁到函数结束（除非 wait 临时释放）

    // wait接收两个参数，第一个参数是unique_lock<mutex>，第二个参数是谓词，谓词为真时，线程继续执行。
    cv.wait(lk, [] { return stage >= 2; }); // 直到 second 把 stage 置为 2
    cout << "third" << endl;               // endl：换行并刷新输出缓冲区
}

/*
 * main：程序入口；返回 int，惯例用 0 表示正常结束。
 * thread t1(first)：创建线程对象 t1，子线程将执行函数 first（函数名即函数指针/可调用对象）
 * join()：阻塞主线程，直到对应子线程执行完；避免主程序先退出而子线程还在跑。
 */
int main() {
    cout << "[main] 主线程开始：即将创建三个线程（执行顺序仍由 first/second/third 内同步保证）" << endl;

    cout << "[main] 创建 t1(first) …" << endl;
    thread t1(first);
    cout << "[main] 创建 t2(second) …" << endl;
    thread t2(second);
    cout << "[main] 创建 t3(third) …" << endl;
    thread t3(third);
    cout << endl;

    cout << "[main] 三个线程已启动，下面 join 会阻塞主线程直到各子线程结束" << endl;
    cout << endl;
    t1.join();
    t2.join();
    t3.join();
    cout << endl;
    cout << "[main] t3 已结束，全部子线程执行完毕" << endl;

    return 0;
}
