#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

mutex mtx;
vector<int> shared_data;

void add_data(int value) {
    lock_guard<mutex> lock(mtx);
    shared_data.push_back(value);
    cout << "Added " << value << " to shared data" << endl;
}

void print_data() {
    lock_guard<mutex> lock(mtx);
    cout << "Shared data: ";
    for (int value : shared_data) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<thread> threads;
    
    // 创建多个线程添加数据
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(add_data, i);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    // 打印最终数据
    print_data();
    
    return 0;
}