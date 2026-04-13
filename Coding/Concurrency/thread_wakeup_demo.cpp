// ============================================================================
// 文件说明：演示 C++ 中「条件变量 + 互斥量」唤醒工作线程的三种常见写法。
// 前置知识：函数、类、lambda、引用、STL 容器 —— 若陌生可先查对应小节。
// ============================================================================

// #include 是预处理指令：在编译前把头文件内容「文本替换」进本文件。
// 尖括号 <> 表示从系统/编译器标准库路径查找头文件。

#include <iostream>           // 标准输入输出流：cin、cout、endl 等
#include <thread>             // std::thread：创建与管理线程；this_thread 命名空间
#include <mutex>              // std::mutex、lock_guard、unique_lock：互斥与加锁
#include <condition_variable> // std::condition_variable：线程间「等待 / 通知」
#include <vector>             // std::vector：动态数组
#include <queue>              // std::queue：队列（FIFO）
#include <functional>         // 本文件未直接使用 std::function，可删；保留也不影响编译

// using namespace std; 表示「默认在 std 命名空间里找名字」，可少写 std::。
// 大项目里常不推荐写在头文件里（会污染全局命名）；单文件练习可以简化书写。
using namespace std;

/*
 * @brief 方法1：使用多个条件变量
 * @details 该方法使用多个条件变量来唤醒多个线程，每个线程都有自己的条件变量和标志位。
 * @note 线程唤醒的顺序是根据标志位的设置顺序，而不是根据线程的创建顺序。
*/
void method1() {
    /*
     * 【method1：线程「谁先继续跑」由谁控制？】
     *
     * 1) 三组「搭档」——变量如何配合
     *    - 线程 t1 只认 (ready1 + cv1)：在 cv1 上 wait，直到谓词 ready1==true。
     *    - 线程 t2 只认 (ready2 + cv2)：在 cv2 上 wait，直到谓词 ready2==true。
     *    - 线程 t3 只认 (ready3 + cv3)：在 cv3 上 wait，直到谓词 ready3==true。
     *    每组里：bool 标志表示「允许通过」；condition_variable 负责「睡眠 / 被叫醒」；
     *    二者必须一起用：只改标志不 notify，对方可能还在睡；只 notify 不改标志，wait 醒来后发现谓词仍假会继续睡。
     *
     * 2) mtx 的作用——把「改标志」和「读标志」串起来
     *    主线程用 lock_guard 写 ready*；子线程在 wait 前后由 unique_lock 持有同一把 mtx。
     *    这样不会出现「半个 bool 被写到一半」之类的数据竞争；并与条件变量的使用约定一致。
     *
     * 3) 执行顺序（谁先打印）≠ 线程创建顺序
     *    t1、t2、t3 几乎同时创建并卡在各自的 wait 上；真正决定打印先后的是主线程下面三段：
     *    先 ready2+notify cv2 → 再 ready1+notify cv1 → 再 ready3+notify cv3。
     *    所以控制台里会看到「线程2 → 线程1 → 线程3」，而不是按 t1、t2、t3 编号顺序。
     *
     * 4) sleep_for(500ms) 的作用
     *    只是人为拉开三次「置位 + 唤醒」的时间间隔，让输出顺序更易读；不是并发正确性所必需。
     *    若去掉 sleep，三次唤醒仍按代码书写顺序发生，但打印可能挤在一起。
     */
    // endl 输出换行并刷新缓冲区；<< 是「流插入运算符」，链式向左结合。
    cout << "=== 方法1：使用多个条件变量 ===" << endl;

    // mutex mtx：定义名为 mtx 的互斥量对象；类型是 std::mutex。
    // 作用：同一时刻最多一个线程能「持有」与它关联的锁，保护共享数据。
    mutex mtx;  // 互斥量：保护共享数据；与 lock_guard / unique_lock 配合使用
    // condition_variable：必须配合 mutex；线程 wait，另一线程 notify。
    condition_variable cv1, cv2, cv3;

    // bool 就绪标志；初值 false 表示尚未允许对应线程通过 wait。
    bool ready1 = false, ready2 = false, ready3 = false;

    /*
     * 对照表（把「谁创建 / 在哪等 / 什么条件醒 / 谁叫醒」一次看清）：
     *
     *   对象     创建位置              阻塞位置                    通过条件（谓词为真）     主线程叫醒位置（本函数靠下）
     *   --------------------------------------------------------------------------------------------------------
     *   t1   thread t1(...) 这里    cv1.wait(..., ready1)      ready1 == true        第二段：ready1=true + cv1.notify_one()
     *   t2   thread t2(...) 这里    cv2.wait(..., ready2)      ready2 == true        第一段：ready2=true + cv2.notify_one()
     *   t3   thread t3(...) 这里    cv3.wait(..., ready3)      ready3 == true        第三段：ready3=true + cv3.notify_one()
     *
     * 说明：三个子线程几乎同时跑到各自的 wait 里睡下；主线程按代码顺序改 ready 并 notify，所以打印顺序是 2→1→3，
     *       与 t1、t2、t3 的书写顺序（1→2→3）无关。
     */
    // thread t1(...)：构造线程对象 t1，参数是可调用对象（这里是 lambda），新线程立刻开始执行该 lambda。
    // [&](){ ... }：lambda 语法 —— [] 是捕获列表，& 表示按引用捕获外部变量（可读写 mtx、cv1、ready1 等）。
    // () 是参数列表，此处无参数；{ } 是函数体，在新线程里运行。
    thread t1([&]() {   // t1里面是一个lambda表达式，用于等待ready1标志位为true，
        // unique_lock<mutex> lock(mtx)：在 mtx 上「加锁」；析构时自动解锁（RAII）。作用范围 = 外层 lambda 的整个大括号
        // condition_variable::wait 要求配合 unique_lock，不能用 lock_guard（因 wait 要临时解锁）。
        unique_lock<mutex> lock(mtx);
        cout << "[t1] 子线程已运行：已加锁，即将在 cv1 上等待；唤醒条件：ready1 == true（由主线程稍后写入）" << endl;
        // wait(lock, predicate)：原子地「若谓词为假则等待」；被唤醒后会再次检查谓词，避免虚假唤醒。
        // [&]{ return ready1; }：谓词 lambda，捕获引用，返回 ready1 是否为 true。
        cv1.wait(lock, [&]{ return ready1; });   // 等待ready1标志位为true，谓词为真时，线程继续执行
        cout << "[t1] wait 已返回（谓词为真），线程1被唤醒，执行任务" << endl;
    });

    thread t2([&]() {
        unique_lock<mutex> lock(mtx);    // mtx这里的作用域 = 外层 lambda 的整个大括号
        cout << "[t2] 子线程已运行：已加锁，即将在 cv2 上等待；唤醒条件：ready2 == true" << endl;
        cv2.wait(lock, [&]{ return ready2; });    // 等待ready2标志位为true，谓词为真时，线程继续执行
        cout << "[t2] wait 已返回，线程2被唤醒，执行任务" << endl;
    });

    thread t3([&]() {
        unique_lock<mutex> lock(mtx);
        cout << "[t3] 子线程已运行：已加锁，即将在 cv3 上等待；唤醒条件：ready3 == true" << endl;
        cv3.wait(lock, [&]{ return ready3; });    // 等待ready3标志位为true
        cout << "[t3] wait 已返回，线程3被唤醒，执行任务" << endl;
    });


    // ---------- 主线程：按「写标志 → notify 对应 cv」的顺序驱动三个子线程 ----------
    // 下面三段与「对照表」一一对应：每一段都是 在锁内改 bool → 解锁 → notify 正确的 cv，叫醒「正在该 cv 上 wait」的那个线程。
    // this_thread::sleep_for：让「当前线程」（这里是主线程）睡眠一段时间。主线程是 main 函数所在的线程。
    // chrono::milliseconds(500)：表示 500 毫秒；chrono 通常随 <thread> 间接可用，可显式 #include <chrono> 更严谨。
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "\n[主线程] 三个子线程应已全部卡在各自的 cv*.wait 上；下面分三步手动决定「谁先醒」（本例顺序 2→1→3）\n" << endl;

    // 单独花括号 { } 构成作用域：让 lock_guard 在块结束时析构，从而尽早释放锁。
    // 【第 1 步】对应上方 thread t2 / cv2.wait：先把 ready2 置真，再 notify cv2 → 只有 t2 的谓词变真并被点名唤醒。
    {
        // lock_guard：构造时 lock(mtx)，析构时 unlock；比 unique_lock 轻，但不能用于 condition_variable::wait。
        lock_guard<mutex> lock(mtx);
        ready2 = true;  // 与 cv2、t2 配对：先允许「线程2 那一侧」的谓词为真
    }
    cout << "[主线程] 第1步：ready2=true（锁外即将 notify）；目标：唤醒正在 cv2 上等的 t2" << endl;
    cv2.notify_one();  // 叫醒在 cv2 上阻塞的 t2（通常只对应一个）；与 ready2 配套使用

    this_thread::sleep_for(chrono::milliseconds(500));
    // 【第 2 步】对应 thread t1 / cv1.wait：ready1=true + cv1.notify_one() → 唤醒 t1。
    {
        lock_guard<mutex> lock(mtx);
        ready1 = true;  // 与 cv1、t1 配对
    }
    cout << "[主线程] 第2步：ready1=true；目标：唤醒在 cv1 上等的 t1" << endl;
    cv1.notify_one();

    this_thread::sleep_for(chrono::milliseconds(500));
    // 【第 3 步】对应 thread t3 / cv3.wait：ready3=true + cv3.notify_one() → 唤醒 t3。
    {
        lock_guard<mutex> lock(mtx);
        ready3 = true;  // 与 cv3、t3 配对
    }
    cout << "[主线程] 第3步：ready3=true；目标：唤醒在 cv3 上等的 t3" << endl;
    cv3.notify_one();
    // 小结：子线程打印顺序 2→1→3，由主线程三次「ready* 与 cv*」的书写顺序决定，不是由 t1/t2/t3 创建顺序决定。

    // join()：阻塞当前线程（主线程），直到对应子线程执行结束；必须 join 或 detach，否则 std::thread 析构会 terminate。
    // 必须join，否则主线程结束后，子线程会继续执行，导致程序崩溃。而detach会使得子线程成为孤儿线程，不会被主线程等待，也不会被主线程结束。
    // 使用join则主线程会等待子线程结束，然后主线程继续执行，不会出现子线程继续执行的情况。
    t1.join();  // 等待t1线程结束
    t2.join();  // 等待t2线程结束
    t3.join();  // 等待t3线程结束
    cout << endl;
}

/*
 * method2：一个 condition_variable + 多个「按线程编号」的就绪标志（与 method1 对比）
 * ---------------------------------------------------------------------------
 * method1 是每个线程独占一个 cv + 一个 ready，用 notify_one 精确叫醒一个。
 * method2 是三个线程共用一个 cv，用 thread_ready[i] 区分「第 i 号线程可不可以往下走」；
 * 叫醒时用 notify_all 把在 cv 上等的线程全部唤醒，但只有谓词 thread_ready[i]==true 的那个会
 * 真正通过 wait 返回，其余线程谓词仍为假，会再次阻塞在 wait 里（不会误跑下去）。
 */
void method2() {
    cout << "=== 方法2：使用线程特定标记 ===" << endl;

    // mtx：保护 thread_ready 的读写，与所有子线程里的 unique_lock 使用同一把锁。
    mutex mtx;
    // 全局只有一个 cv：所有子线程都在「同一个」condition_variable 上 wait。
    condition_variable cv;
    // thread_ready[j]==true 表示「第 j 号工作线程允许通过 wait」；初值全 false，三个线程起先都会阻塞。
    // vector<bool>(3, false)：长度为 3，每个元素 false。注意 vector<bool> 是特化，存储方式与 vector<int> 不同。
    vector<bool> thread_ready(3, false);

    // threads：稍后 for 循环里 emplace_back 进 3 个 thread；不能拷贝 thread，只能移动或 emplace。
    // emplace_back：原地构造 thread，直接传入可调用对象（lambda）。会直接在threads的末尾构造一个thread对象，而不是先构造一个thread对象，再将其移动到threads的末尾。
    vector<thread> threads; // threads类型是vector<thread>，用于存储线程对象，vector存储的是线程对象的指针。
    /*
     * 循环创建 3 个子线程，下标 i=0,1,2 对应「线程1/2/3」的业务含义（输出里用 i+1）。
     *
     * 对照表（创建 / 等待 / 条件 / 主线程哪一步置位）：
     *   i   emplace_back 第 i 个      阻塞于              通过 wait 的条件           主线程置 thread_ready[i]=true 的顺序（本例）
     *   --------------------------------------------------------------------------------------------------------
     *   0   第 1 个线程               cv.wait             thread_ready[0]==true      第 2 步
     *   1   第 2 个线程               同上                thread_ready[1]==true      第 1 步（最先）
     *   2   第 3 个线程               同上                thread_ready[2]==true      第 3 步
     * 因此打印顺序仍是「线程2 → 线程1 → 线程3」，由主线程写 thread_ready 的顺序决定，不是 for 里 i 的顺序。
     */
    for (int i = 0; i < 3; i++) {
        // emplace_back：原地构造 thread，直接传入可调用对象（lambda）。
        // [&, i]()：外层变量 mtx、cv、thread_ready 按引用捕获；循环变量 i 必须按值捕获进 lambda，
        //           否则三次循环结束后 i 会变成 3，三个 lambda 若共享同一个 i 会全部变成「等 thread_ready[3]」（越界/逻辑错误）。
        threads.emplace_back([&, i]() {
            unique_lock<mutex> lock(mtx);  // 与主线程改 thread_ready 时用同一把锁配对，使用unique_lock而不是lock_guard是因为wait需要临时解锁
            cout << "[方法2][i=" << i << "，对应业务「线程" << i+1 << "」] 已加锁，即将在「共享」cv 上 wait；通过条件：thread_ready[" << i << "]==true" << endl;
            // 谓词：thread_ready[i] 为真才返回；假则阻塞。notify_all 后所有等待线程都会被检查谓词，仅满足的继续。
            cv.wait(lock, [&, i]{ return thread_ready[i]; });
            cout << "[方法2][i=" << i << "] wait 已返回（谓词为真），线程" << i+1 << "被唤醒，执行任务" << endl;
        });
    }

    // ---------- 主线程：按顺序把 thread_ready[?] 置 true，并 notify_all；顺序决定谁先打印 ----------
    /*
     * 下面每一小段都是：{ lock_guard<mutex> lock(mtx); 改 thread_ready; }
     * - 锁的是本函数开头的 mutex mtx，与子线程里 unique_lock<mutex> lock(mtx) 是同一把互斥量。
     * - lock_guard 在「进入花括号」时 lock(mtx)，在「离开花括号」时析构并自动 unlock(mtx)（RAII）。
     * - 因此执行到后面的 cv.notify_all() 时，主线程已不持有 mtx；被唤醒的子线程才能在 wait 返回后顺利再次锁住 mtx。
     *   （输出里写「mtx 已解锁」指的就是这把锁，不是 cv。）
     */
    this_thread::sleep_for(chrono::milliseconds(500));  // 可选：给三个子线程时间全部进入 wait
    cout << "\n[主线程·方法2] 三个子线程应已全部阻塞在同一 cv 上；下面三步依次放行 thread_ready[1]→[0]→[2]，打印顺序将为 2→1→3\n" << endl;

    // 第 1 步：在持 mtx 下写入 thread_ready[1]，出 { } 后 mtx 已解锁，再 notify。
    {
        lock_guard<mutex> lock(mtx);    // lock_guard在进入花括号时lock(mtx)，在离开花括号时析构并自动unlock(mtx)（RAII）。
        // 锁的是本函数开头的mutex mtx，与子线程里unique_lock<mutex> lock(mtx)是同一把互斥量。
        // 加锁目的：保护thread_ready的读写，与所有子线程里的unique_lock使用同一把锁。
        // 锁生效时只有主线程能读写thread_ready，其他线程不能读写。
        thread_ready[1] = true;
    }
    cout << "[主线程·方法2] 第1步：thread_ready[1]=true，mtx 已在 { } 结束时解锁，接着 notify_all；目标：仅 i=1（线程2）通过 wait" << endl;
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(500));

    // 第 2 步：同上，放行 i==0（线程1）。
    {
        lock_guard<mutex> lock(mtx);
        thread_ready[0] = true;
    }
    cout << "[主线程·方法2] 第2步：thread_ready[0]=true，mtx 已解锁，notify_all；目标：仅 i=0（线程1）通过 wait" << endl;
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(500));

    // 第 3 步：同上，放行 i==2（线程3）。
    {
        lock_guard<mutex> lock(mtx);
        thread_ready[2] = true;
    }
    cout << "[主线程·方法2] 第3步：thread_ready[2]=true，mtx 已解锁，notify_all；目标：仅 i=2（线程3）通过 wait" << endl;
    cv.notify_all();

    // join：按容器顺序等待线程结束；顺序与「谁先被唤醒」无关，这里只是回收三个 thread。
    // 因为前面是emplace_back进的vector，所以这里使用for循环等待线程结束。
    for (auto& t : threads) {
        t.join();
    }
    cout << endl;
}

/*
 * method3：一个 condition_variable + 三条任务队列 + 全局 done（与 method1/2 对比）
 * ---------------------------------------------------------------------------
 * method1/2 里子线程通常「等一次条件」就跑完；method3 里工作线程用 while 循环，可多次被唤醒，
 * 每次从自己的 task_queues[i] 里 pop 一个任务并处理，直到主线程把 done 置 true 且本队列为空才退出。
 * 所有线程共用一个 cv：notify_all 后，每个线程都会检查自己的谓词（本队列非空 或 done），
 * 满足的继续往下走，不满足的继续 wait。
 */
void method3() {
    cout << "=== 方法3：使用任务队列 ===" << endl;

    mutex mtx;                      // 保护 task_queues、done 的读写
    condition_variable cv;          // 共享：谁在等任务/等结束都用它
    // task_queues[i] 只给第 i 号线程消费；其它线程不会 pop 别人的队列（约定，靠逻辑保证）。
    // 类型是vector<queue<int>>，用于存储3个queue<int>，queue<int>用于存储int类型的数据。
    // 3个queue<int>分别对应3个线程，每个线程从自己的queue<int>中取任务。
    // 主线程向task_queues[k] push数字任务，然后notify_all，叫醒所有线程，每个线程都会检查自己的谓词（本队列非空 或 done），满足的继续往下走，不满足的继续wait。
    // vector存储的是queue<int>的指针，所以这里使用for循环等待线程结束。task_queues(3)是在创建3个queue<int>对象，并存储到vector中。
    vector<queue<int>> task_queues(3);
    bool done = false;              // true：主线程宣布「不再投递新任务」，各线程排空本队列后退出

    // threads类型是vector<thread>，用于存储线程对象，vector存储的是线程对象的指针。
    // emplace_back：原地构造 thread，直接传入可调用对象（lambda）。会直接在threads的末尾构造一个thread对象，而不是先构造一个thread对象，再将其移动到threads的末尾。
    vector<thread> threads;
    /*
     * 对照表（创建 / 等待 / 谓词 / 主线程如何驱动）：
     *   i   线程职责                         cv.wait 谓词（为真才返回）
     *   ------------------------------------------------------------------
     *   0   只从 task_queues[0] 取任务      !q0.empty() || done
     *   1   只从 task_queues[1] 取          !q1.empty() || done
     *   2   只从 task_queues[2] 取          !q2.empty() || done
     * 主线程：向 task_queues[k] push 数字任务 → notify_all；最后 done=true → notify_all，各线程醒来发现
     *         done 且自己队列为空则 break 退出。
     */
    for (int i = 0; i < 3; i++) {
        // [&, i]()：外层变量 mtx、cv、task_queues 按引用捕获；循环变量 i 必须按值捕获进 lambda，
        //           否则三次循环结束后 i 会变成 3，三个 lambda 若共享同一个 i 会全部变成「等 task_queues[3]」（越界/逻辑错误）。
        threads.emplace_back([&, i]() {
            cout << "[方法3][i=" << i << "，业务线程" << i+1 << "] 启动：while 循环，从 task_queues[" << i << "] 取任务直到 done 且队列为空" << endl;
            while (!done) {
                // 这里加锁是为了保护task_queues的读写，与主线程里的unique_lock使用同一把锁。
                unique_lock<mutex> lock(mtx);
                cout << "[方法3][i=" << i << "] 已加锁，即将在共享 cv 上 wait；谓词：!task_queues[" << i << "].empty() || done" << endl;
                cv.wait(lock, [&, i]{ return !task_queues[i].empty() || done; });
                // 这里wait是为了等待task_queues[i].empty()为true或done为true，满足的继续往下走，不满足的继续wait。

                if (done && task_queues[i].empty()) {
                    cout << "[方法3][i=" << i << "] wait 返回：done==true 且本队列空 → 退出 while" << endl;
                    break;
                }

                if (!task_queues[i].empty()) {  // 如果队列非空，则取任务
                    int task = task_queues[i].front();  // 取任务
                    task_queues[i].pop();  // 取任务后，任务从队列中移除
                    lock.unlock();  // 解锁是为了让其他线程可以访问task_queues，因为wait会临时解锁。

                    cout << "[方法3][i=" << i << "] wait 返回后取到任务 → 线程" << i+1 << "处理任务：" << task << endl;
                }
            }
        });
    }

    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "\n[主线程·方法3] 三子线程应已进入各自 wait；下面依次 push 任务并 notify_all（可叫醒多个线程，各自检查自己的队列）\n" << endl;

    {
        lock_guard<mutex> lock(mtx);
        task_queues[0].push(101);
    }
    cout << "[主线程·方法3] 第1次：task_queues[0].push(101) + notify_all → 通常仅 i=0 会继续执行" << endl;
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(500));
    {
        lock_guard<mutex> lock(mtx);
        task_queues[1].push(201);
    }
    cout << "[主线程·方法3] 第2次：task_queues[1].push(201) + notify_all" << endl;
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(500));
    {
        lock_guard<mutex> lock(mtx);
        task_queues[2].push(301);
    }
    cout << "[主线程·方法3] 第3次：task_queues[2].push(301) + notify_all" << endl;
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(500));
    {
        lock_guard<mutex> lock(mtx);
        task_queues[0].push(102);
        task_queues[0].push(103);
    }
    cout << "[主线程·方法3] 第4次：向队列0 连续 push 102、103 + notify_all（线程1可能连续处理多个任务）" << endl;
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(500));
    {
        lock_guard<mutex> lock(mtx);
        done = true;
    }
    cout << "[主线程·方法3] 结束阶段：done=true + notify_all → 各线程谓词为真，排空本队列后退出" << endl;
    cv.notify_all();

    for (auto& t : threads) {
        t.join();
    }
    cout << endl;
}

// int main()：程序入口；返回 int，惯例 0 表示成功（给操作系统/调用者）。
int main() {
    cout << "线程唤醒方法演示" << endl << endl;

    method1();  // 调用三个演示函数，顺序执行（每个内部已 join，故 method2 在 method1 完全结束后才跑）
    method2();
    method3();

    cout << "所有方法演示完成" << endl;
    return 0;  // 向环境返回 0；可省略（C++ 里 main 若省略 return，等价于 return 0）
}
