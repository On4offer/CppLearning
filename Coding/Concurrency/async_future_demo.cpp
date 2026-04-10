#include <iostream>
#include <future>
#include <thread>
using namespace std;

int calculate(int a, int b) {
    this_thread::sleep_for(chrono::seconds(2));
    return a + b;
}

void task() {
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Task completed" << endl;
}

int main() {
    // 异步计算
    future<int> result = async(launch::async, calculate, 5, 3);
    cout << "Calculating..." << endl;
    cout << "Result: " << result.get() << endl;
    
    // 异步任务
    future<void> task_result = async(launch::async, task);
    cout << "Task started..." << endl;
    task_result.wait();
    
    return 0;
}