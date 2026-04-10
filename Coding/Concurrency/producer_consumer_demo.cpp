// 包含输入输出库，用于cout打印
#include <iostream>
// 包含线程库，用于创建和管理线程
#include <thread>
// 包含互斥量库，用于线程同步
#include <mutex>
// 包含条件变量库，用于线程间通信
#include <condition_variable>
// 包含队列容器库，用于存储数据
#include <queue>
// 使用std命名空间，避免每次都写std::
using namespace std;

// 全局互斥量，用于保护共享数据的访问
mutex mtx;
// 条件变量，用于线程间通信（通知机制）
condition_variable cv;
// 共享数据队列，用于存储生产者生产的数据
queue<int> data_queue;
// 标志变量，指示生产者是否完成生产
bool done = false;

// 生产者函数：负责生产数据并放入队列
void producer() {
    // 生产10个数据
    for (int i = 0; i < 10; i++) {
        // 作用域：限制锁的范围
        { 
            // 创建lock_guard对象，自动管理锁的获取和释放
            lock_guard<mutex> lock(mtx);
            // 将数据放入队列
            data_queue.push(i);
            // 输出生产信息
            cout << "Produced: " << i << endl;
        } // 自动释放锁
        
        // 通知等待的消费者线程
        cv.notify_one();
        // 模拟生产过程的耗时
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    // 生产完成后，设置done标志
    { 
        lock_guard<mutex> lock(mtx);
        done = true;
    } // 自动释放锁
    
    // 通知消费者生产已完成
    cv.notify_one();
}

// 消费者函数：负责从队列中获取并消费数据
void consumer() {
    // 无限循环，直到生产完成且队列为空
    while (true) {
        // 创建unique_lock对象，用于条件变量的等待
        unique_lock<mutex> lock(mtx);
        
        // 等待条件：队列不为空或生产已完成
        // 第一个参数是锁对象，第二个参数是条件函数
        cv.wait(lock, []{ return !data_queue.empty() || done; });
        
        // 检查是否生产完成且队列为空
        if (done && data_queue.empty()) {
            break; // 退出循环
        }
        
        // 从队列中获取数据
        int value = data_queue.front();
        data_queue.pop(); // 从队列中移除
        
        // 输出消费信息
        cout << "Consumed: " << value << endl;
    } // 自动释放锁
}

// 主函数
int main() {
    // 创建生产者线程
    thread producer_thread(producer);
    // 创建消费者线程
    thread consumer_thread(consumer);
    
    // 等待生产者线程完成
    producer_thread.join();
    // 等待消费者线程完成
    consumer_thread.join();
    
    // 输出完成信息
    cout << "Done" << endl;
    
    // 返回0表示程序正常结束
    return 0;
}