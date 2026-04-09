#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 静态内存分配
    int static_var = 10;
    
    // 栈内存分配
    int stack_var = 20;
    
    // 堆内存分配
    int* heap_var = new int(30);
    cout << "*heap_var = " << *heap_var << endl;
    delete heap_var; // 释放堆内存
    
    // 数组的堆内存分配
    int* heap_array = new int[5]{1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        cout << heap_array[i] << " ";
    }
    cout << endl;
    delete[] heap_array; // 释放数组内存
    
    // 使用智能指针
    // unique_ptr 是独占所有权的智能指针，离开作用域时自动释放内存。
    unique_ptr<int> up(new int(40));
    // 推荐写法：使用 make_unique 创建 unique_ptr，避免直接使用 new。
    // unique_ptr<int> up = make_unique<int>(40);
    // 还可以先声明空指针，然后再 reset：
    // unique_ptr<int> up2;
    // up2.reset(new int(40));
    // 也可以创建数组形式的 unique_ptr：
    // unique_ptr<int[]> arr = make_unique<int[]>(5);
    cout << "*up = " << *up << endl;
    // unique_ptr 不允许拷贝，但可以移动所有权：
    // unique_ptr<int> up2 = move(up); // up 变为空，up2 拥有该对象
    
    // shared_ptr 是共享所有权的智能指针，多个 shared_ptr 可以指向同一块内存。
    shared_ptr<int> sp(new int(50));
    // 推荐写法：使用 make_shared 创建 shared_ptr，通常更高效且更安全。
    // shared_ptr<int> sp = make_shared<int>(50);
    cout << "*sp = " << *sp << endl;
    // shared_ptr 可以拷贝，共享内存对象：
    // shared_ptr<int> sp2 = sp; // sp 与 sp2 共享对象所有权
    
    // weak_ptr 用于观察 shared_ptr 管理的对象，但不增加引用计数。
    // weak_ptr<int> wp = sp; // wp 不拥有对象，只是一个观察者
    
    return 0;
}