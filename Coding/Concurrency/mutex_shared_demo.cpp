// 包含输入输出库，用于cout打印
#include <iostream>
// 包含线程库，用于创建和管理线程
#include <thread>
// 包含互斥量库，用于线程同步
#include <mutex>
// 包含向量容器库，用于存储数据和线程对象
#include <vector>
// 使用std命名空间，避免每次都写std::
using namespace std;

// 全局互斥量，用于保护共享数据的访问
mutex mtx;
// 共享数据向量，多个线程会对其进行操作
vector<int> shared_data;

// 向共享数据中添加值的函数
// @param value 要添加的值
void add_data(int value) {
    // 创建lock_guard对象，自动管理锁的获取和释放
    // 构造时获取锁，析构时自动释放锁
    lock_guard<mutex> lock(mtx);
    // 向共享数据中添加值
    shared_data.push_back(value);
    // 输出添加成功的信息
    cout << "Added " << value << " to shared data" << endl;
}

// 打印共享数据的函数
void print_data() {
    // 创建lock_guard对象，自动管理锁的获取和释放
    lock_guard<mutex> lock(mtx);
    // 输出提示信息
    cout << "Shared data: ";
    // 遍历共享数据并输出每个值
    for (int value : shared_data) {
        cout << value << " ";
    }
    // 输出换行符
    cout << endl;
}

// 主函数
int main() {
    // 用于存储线程对象的向量
    cout << "Starting threads..." << endl;
    vector<thread> threads;
    
    // 创建多个线程添加数据
    cout << "Creating threads..." << endl;
    for (int i = 0; i < 10; i++) {
        // 在向量末尾直接构造线程对象，执行add_data函数，参数为i
        cout << "Creating thread " << i << "..." << endl;
        threads.emplace_back(add_data, i);  //  在容器末尾，直接用参数构造一个新元素。执行add_data函数，参数为i
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        // 阻塞当前线程，直到目标线程执行完毕
        cout << "Joining thread " << t.get_id() << "..." << endl;
        t.join();
        cout << "Thread " << t.get_id() << " joined" << endl;  
    }
    cout << "All threads joined" << endl;
    
    // 打印最终数据
    print_data();
    
    // 返回0表示程序正常结束
    return 0;
}