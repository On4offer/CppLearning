#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
using namespace std;

atomic<int> counter(0);

void increment(int id, int times) {
    for (int i = 0; i < times; i++) {
        counter++;
        cout << "Thread " << id << " incremented counter to " << counter << endl;
    }
}

int main() {
    vector<thread> threads;
    
    // 创建5个线程
    for (int i = 0; i < 5; i++) {
        threads.emplace_back(increment, i, 10);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "Final counter value: " << counter << endl;
    
    return 0;
}