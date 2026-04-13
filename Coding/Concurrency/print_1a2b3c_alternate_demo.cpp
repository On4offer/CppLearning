// ============================================================================
// 两线程交替打印 1A2B3C4D…
// - 线程 A（print_number）：只负责打印数字 1, 2, 3, …
// - 线程 B（print_letter）：只负责打印字母 A, B, C, …
// 目标输出（无空格）：1A2B3C… 共 PAIRS 组「数字+字母」，即 2*PAIRS 个字符。
//
// 思路：用全局步数 seq（从 0 计）表示「下一个该打印的是第几个字符」：
//   seq=0 → 打数字 1；seq=1 → 打字母 A；seq=2 → 打数字 2；seq=3 → 打字母 B；…
//   偶数 seq：轮到数字线程；奇数 seq：轮到字母线程。
//   第 k 组（k 从 0 开始）：数字为 (k+1)，字母为 'A'+k，对应 seq=2k 与 seq=2k+1，打印时用 seq/2 对齐到组号 k。
// ============================================================================

#include <iostream>           // cout、endl
#include <thread>             // std::thread
#include <mutex>              // mutex、unique_lock
#include <condition_variable> // condition_variable::wait / notify_all

// using namespace std：在 .cpp 单文件练习中可少写 std::；大项目头文件里一般不用，避免名字污染
using namespace std;

// ---------- 线程间共享状态（必须用同一把 mutex 保护读写）----------
mutex mtx;                     // 互斥量：保证任一时刻只有一个线程在修改 seq 或读 seq 做判断
condition_variable cv;         // 条件变量：一方改完 seq 后 notify，另一方从 wait 醒来

// seq：下一个待打印的「全局序号」0 .. 2*PAIRS-1；每打印一个字符后 seq++。
// 结束：seq 增至 2*PAIRS 时，两线程的 wait 谓词里用 seq>=total_steps 唤醒彼此以便退出循环。
int seq = 0;

// PAIRS：要输出多少组「数字+字母」；总字符数 = 2 * PAIRS（可改此常量重编译）
const int PAIRS = 6;

/*
 * print_number：只在 seq 为偶数且未满时打印当前组对应的数字，然后 seq++。
 * wait 谓词含义：要么已经全部打完（seq>=total_steps，需要醒来以便退出 while），
 *               要么还没打完且当前轮到数字（seq 偶数）。
 */
void print_number() {
    const int total_steps = 2 * PAIRS;  // 总步数 = 2 * PAIRS，作为wait谓词的判断条件。
    while (true) {
        unique_lock<mutex> lk(mtx);  // 与 cv.wait 配合必须用 unique_lock

        // [&]：按引用捕获外部变量（seq、total_steps 等），lambda 内可读写到真实的 seq
        // cv.wait(lk, [&] { ... })：在 lk 上等待，谓词为真时，线程继续执行。谓词为假时，线程阻塞，等待notify_all唤醒且再次检查谓词。
        // 线程在这里阻塞等待，直到【满足下面两个条件之一】才会醒来继续执行
        cv.wait(lk, [&] {
            // seq >= total_steps：已全部输出，退出线程函数
            // 或者：seq < total_steps && seq % 2 == 0：还没打完且当前轮到数字（seq 偶数），继续打印数字
            // return true表示谓词为真，线程继续执行。谓词为假时，线程阻塞，等待notify_all唤醒且再次检查谓词。
            return seq >= total_steps || (seq < total_steps && seq % 2 == 0);
        });
        if (seq >= total_steps) break;   // 已全部输出，退出线程函数

        // 当前 seq 为偶数 2k：应打印第 (k+1) 个数字，即 seq/2+1
        cout << (seq / 2 + 1);
        ++seq;
        lk.unlock();           // 尽早释锁，再通知另一线程（与「临界区只包住必要部分」一致）
        cv.notify_all();       // 另一线程可能在 wait，需要唤醒检查谓词
    }
}

/*
 * print_letter：只在 seq 为奇数且未满时打印当前组字母，然后 seq++。
 * 与 print_number 对称；谓词为「结束」或「轮到字母」（seq 奇数）。
 */
void print_letter() {
    const int total_steps = 2 * PAIRS;
    while (true) {
        unique_lock<mutex> lk(mtx);
        cv.wait(lk, [&] {
            // 谓词为真时，线程继续执行。谓词为假时，线程阻塞。
            // seq >= total_steps：已全部输出，退出线程函数
            // seq < total_steps && seq % 2 == 1：还没打完且当前轮到字母（seq 奇数）
            return seq >= total_steps || (seq < total_steps && seq % 2 == 1);
        });
        if (seq >= total_steps) break;

        // 当前 seq 为奇数 2k+1：与上一打印的数字同属第 k 组，字母为 'A'+k；整数除法 seq/2 即为 k
        cout << char('A' + seq / 2);
        ++seq;
        lk.unlock();
        cv.notify_all();
    }
}

/*
 * main：创建两个线程分别跑数字 / 字母逻辑，join 等待结束。
 * 输出顺序由 seq 与两个谓词共同保证，与 thread 构造顺序无关。
 */
int main() {
    cout << "共 " << PAIRS << " 组数字+字母（共 " << 2 * PAIRS << " 个字符）: " << endl;
    thread t_num(print_number);
    thread t_ch(print_letter);

    t_num.join();
    t_ch.join();


    cout << endl;
    cout << "两线程均已 join，结束。" << endl;
    return 0;
}
