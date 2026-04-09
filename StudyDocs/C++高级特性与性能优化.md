# C++高级特性与性能优化

本文档涵盖C++的高级特性和性能优化技术，包括C++11/14/17/20的新特性、内存管理、并发编程、性能分析等内容，结合代码示例，帮助您在校招笔试面试中掌握C++高级特性和性能优化相关知识。

## 一、C++11新特性

### 1. 自动类型推导

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // auto 自动推导类型
    auto i = 10;           // i 为 int 类型
    auto d = 3.14;         // d 为 double 类型
    auto s = "Hello";
    // s 为 const char* 类型
    
    //   推导表达式类型
    int a = 10;
    decltype(a) b = 20;    // b 为 int 类型
    decltype(a + b) c = 30; // c 为 int 类型
    
    // 与容器结合使用
    vector<int> v = {1, 2, 3, 4, 5};
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    // C++11 范围 for 循环
    for (auto num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
```

### 2. 智能指针

```cpp
#include <iostream>
#include <memory>
using namespace std;

class MyClass {
public:
    MyClass() {
        cout << "MyClass constructed" << endl;
    }
    ~MyClass() {
        cout << "MyClass destructed" << endl;
    }
    void doSomething() {
        cout << "Doing something" << endl;
    }
};

int main() {
    // unique_ptr：独占所有权
    unique_ptr<MyClass> p1(new MyClass());
    p1->doSomething();
    // unique_ptr 不能复制，只能移动
    unique_ptr<MyClass> p2 = move(p1);
    if (!p1) {
        cout << "p1 is null" << endl;
    }
    p2->doSomething();
    
    // shared_ptr：共享所有权
    shared_ptr<MyClass> p3(new MyClass());
    cout << "p3 use_count: " << p3.use_count() << endl;
    shared_ptr<MyClass> p4 = p3;
    cout << "p3 use_count: " << p3.use_count() << endl;
    cout << "p4 use_count: " << p4.use_count() << endl;
    p3->doSomething();
    p4->doSomething();
    
    // weak_ptr：不增加引用计数
    weak_ptr<MyClass> p5 = p3;
    cout << "p3 use_count: " << p3.use_count() << endl;
    if (auto p6 = p5.lock()) {
        p6->doSomething();
    }
    
    return 0;
}
```

### 3. 右值引用与移动语义

```cpp
#include <iostream>
#include <string>
using namespace std;

class MyString {
private:
    char* data;
    int length;

public:
    // 构造函数
    MyString(const char* str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "Constructor called" << endl;
    }
    
    // 析构函数
    ~MyString() {
        if (data) {
            delete[] data;
            cout << "Destructor called" << endl;
        }
    }
    
    // 复制构造函数，用来复制一个已有的对象，创建新对象
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        cout << "Copy constructor called" << endl;
    }
    
    // 移动构造函数
    MyString(MyString&& other) noexcept {
        length = other.length;
        data = other.data;
        other.data = nullptr;
        other.length = 0;
        cout << "Move constructor called" << endl;
    }
    
    // 复制赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            cout << "Copy assignment called" << endl;
        }
        return *this;
    }
    
    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = other.data;
            other.data = nullptr;
            other.length = 0;
            cout << "Move assignment called" << endl;
        }
        return *this;
    }
    
    const char* c_str() const {
        return data;
    }
};

MyString createString() {
    return MyString("Hello World");
}

int main() {
    // 复制构造
    MyString s1("Hello");
    MyString s2 = s1; // 复制构造
    
    // 移动构造
    MyString s3 = createString(); // 移动构造
    
    // 移动赋值
    MyString s4("Test");
    s4 = move(s1); // 移动赋值
    
    cout << "s2: " << s2.c_str() << endl;
    cout << "s3: " << s3.c_str() << endl;
    cout << "s4: " << s4.c_str() << endl;
    
    return 0;
}
```

### 4.  lambda 表达式

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // 基本 lambda
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "5 + 3 = " << add(5, 3) << endl;
    
    // 捕获外部变量
    int x = 10;
    auto multiply = [x](int a) {
        return a * x;
    };
    cout << "5 * 10 = " << multiply(5) << endl;
    
    // 引用捕获
    int y = 20;
    auto increment = [&y]() {
        y++;
    };
    increment();
    cout << "y = " << y << endl;
    
    // 捕获所有外部变量
    int a = 5, b = 10;
    auto sum = [=]() {
        return a + b;
    };
    cout << "a + b = " << sum() << endl;
    
    // 用于算法
    vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    sort(v.begin(), v.end(), [](int a, int b) {
        return a > b;
    });
    
    cout << "Sorted vector: ";
    for (auto num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
```

### 5.  nullptr

```cpp
#include <iostream>
using namespace std;

void func(int* p) {
    cout << "func(int*) called" << endl;
}

void func(int i) {
    cout << "func(int) called" << endl;
}

int main() {
    // nullptr 是指针类型
    int* p = nullptr;
    func(p);      // 调用 func(int*)
    func(nullptr); // 调用 func(int*)
    func(0);       // 调用 func(int)
    
    // NULL 是宏，可能被定义为 0
    func(NULL);    // 可能调用 func(int)，取决于编译器
    
    return 0;
}
```

### 6. 范围 for 循环

```cpp
#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main() {
    // 遍历 vector
    vector<int> v = {1, 2, 3, 4, 5};
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    // 遍历数组
    int arr[] = {6, 7, 8, 9, 10};
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    
    // 遍历 map
    map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    for (auto& pair : m) {
        cout << pair.first << ": " << pair.second << endl;
    }
    
    // 使用引用避免复制
    for (auto& num : v) {
        num *= 2;
    }
    
    cout << "Modified vector: ";
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
```

### 7. 初始化列表

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class MyClass {
private:
    vector<int> data;

public:
    // 接受初始化列表的构造函数
    MyClass(initializer_list<int> list) {
        for (auto num : list) {
            data.push_back(num);
        }
    }
    
    void print() {
        for (auto num : data) {
            cout << num << " ";
        }
        cout << endl;
    }
};

int main() {
    // 初始化 vector
    vector<int> v = {1, 2, 3, 4, 5};
    
    // 初始化 map
    map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    
    // 初始化自定义类
    MyClass obj = {10, 20, 30, 40, 50};
    obj.print();
    
    return 0;
}
```

### 8. 类型别名

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 使用 typedef
typedef vector<int> IntVector;

// 使用 using (C++11)
typedef void (*FunctionType)(int);
using FunctionType2 = void (*)(int);

// 模板别名
template <typename T>
typedef vector<T> VectorType; // C++11 不支持

template <typename T>
using VectorType2 = vector<T>;

void func(int x) {
    cout << "func called with " << x << endl;
}

int main() {
    // 使用 typedef
    IntVector v = {1, 2, 3, 4, 5};
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    // 使用函数指针类型别名
    FunctionType f1 = func;
    f1(10);
    
    FunctionType2 f2 = func;
    f2(20);
    
    // 使用模板别名
    VectorType2<double> v2 = {1.1, 2.2, 3.3};
    for (double num : v2) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
```

## 二、C++14新特性

### 1. 泛型 lambda

```cpp
#include <iostream>
using namespace std;

int main() {
    // 泛型 lambda
    auto add = [](auto a, auto b) {
        return a + b;
    };
    
    cout << "5 + 3 = " << add(5, 3) << endl;
    cout << "3.14 + 2.71 = " << add(3.14, 2.71) << endl;
    cout << "Hello " + string("World") << endl;
    
    return 0;
}
```

### 2. 变量模板

```cpp
#include <iostream>
#include <type_traits>
using namespace std;

// 变量模板
template <typename T>
constexpr T pi = T(3.14159265358979323846);

// 变量模板特化
template <>
constexpr int pi<int> = 3;

int main() {
    cout << "pi<double>: " << pi<double> << endl;
    cout << "pi<float>: " << pi<float> << endl;
    cout << "pi<int>: " << pi<int> << endl;
    
    return 0;
}
```

### 3. 返回类型推导

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 自动推导返回类型
auto add(int a, int b) {
    return a + b;
}

// 复杂返回类型推导
auto getVector() {
    return vector<int>{1, 2, 3, 4, 5};
}

int main() {
    cout << "add(5, 3) = " << add(5, 3) << endl;
    
    auto v = getVector();
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
```

### 4. 二进制字面量

```cpp
#include <iostream>
using namespace std;

int main() {
    // 二进制字面量
    int a = 0b1010;  // 10
    int b = 0B1100;  // 12
    
    cout << "0b1010 = " << a << endl;
    cout << "0B1100 = " << b << endl;
    
    // 数字分隔符
    int c = 1'000'000;  // 1000000
    cout << "1'000'000 = " << c << endl;
    
    return 0;
}
```

## 三、C++17新特性

### 1. 结构化绑定

```cpp
#include <iostream>
#include <tuple>
#include <map>
using namespace std;

int main() {
    // 绑定 tuple
    auto t = make_tuple(1, 2.5, "Hello");
    auto [x, y, z] = t;
    cout << "x: " << x << ", y: " << y << ", z: " << z << endl;
    
    // 绑定 pair
    pair<int, string> p = {42, "World"};
    auto [a, b] = p;
    cout << "a: " << a << ", b: " << b << endl;
    
    // 绑定 map 元素
    map<string, int> m = {{"a", 1}, {"b", 2}};
    for (auto [key, value] : m) {
        cout << key << ": " << value << endl;
    }
    
    return 0;
}
```

### 2. if constexpr

```cpp
#include <iostream>
#include <type_traits>
using namespace std;

template <typename T>
void print(T t) {
    if constexpr (is_integral_v<T>) {
        cout << "Integral: " << t << endl;
    } else if constexpr (is_floating_point_v<T>) {
        cout << "Floating point: " << t << endl;
    } else {
        cout << "Other type" << endl;
    }
}

int main() {
    print(42);      // 整数
    print(3.14);    // 浮点数
    print("Hello"); // 其他类型
    
    return 0;
}
```

### 3. 折叠表达式

```cpp
#include <iostream>
using namespace std;

// 求和模板
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

// 打印模板
template <typename... Args>
void print(Args... args) {
    (cout << ... << args) << endl;
}

int main() {
    cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << endl;
    print("Hello", " ", "World", "!");
    
    return 0;
}
```

### 4. inline 变量

```cpp
#include <iostream>
using namespace std;

// 内联变量
inline int global_var = 42;

// 模板内联变量
template <typename T>
inline T pi = T(3.14159265358979323846);

int main() {
    cout << "global_var: " << global_var << endl;
    cout << "pi<double>: " << pi<double> << endl;
    
    return 0;
}
```

## 四、C++20新特性

### 1. 概念

```cpp
#include <iostream>
#include <concepts>
using namespace std;

// 定义概念
template <typename T>
concept Integral = is_integral_v<T>;

template <typename T>
concept SignedIntegral = Integral<T> && is_signed_v<T>;

// 使用概念
template <Integral T>
T add(T a, T b) {
    return a + b;
}

// 约束自动类型推导
auto multiply(SignedIntegral auto a, SignedIntegral auto b) {
    return a * b;
}

int main() {
    cout << "add(5, 3) = " << add(5, 3) << endl;
    // cout << add(3.14, 2.71) << endl; // 编译错误，不是整数
    
    cout << "multiply(5, -3) = " << multiply(5, -3) << endl;
    // cout << multiply(5U, 3) << endl; // 编译错误，不是有符号整数
    
    return 0;
}
```

### 2. 范围库

```cpp
#include <iostream>
#include <vector>
#include <ranges>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 过滤偶数并乘以2
    auto result = v | views::filter([](int x) { return x % 2 == 0; })
                    | views::transform([](int x) { return x * 2; });
    
    cout << "Filtered and transformed: ";
    for (auto num : result) {
        cout << num << " ";
    }
    cout << endl;
    
    // 取前5个元素
    auto first5 = v | views::take(5);
    cout << "First 5 elements: ";
    for (auto num : first5) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
```

### 3. 协程

```cpp
#include <iostream>
#include <coroutine>
#include <future>
using namespace std;

// 简单的协程返回类型
struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{handle_type::from_promise(*this)};
        }
        suspend_never initial_suspend() {
            return {};
        }
        suspend_never final_suspend() noexcept {
            return {};
        }
        void return_void() {}
        void unhandled_exception() {}
    };
    
    using handle_type = coroutine_handle<promise_type>;
    handle_type handle;
    
    Task(handle_type h) : handle(h) {}
    ~Task() {
        if (handle) handle.destroy();
    }
    
    void resume() {
        handle.resume();
    }
};

// 协程函数
Task coroutine_example() {
    cout << "Coroutine started" << endl;
    co_await suspend_always{};
    cout << "Coroutine resumed" << endl;
    co_await suspend_always{};
    cout << "Coroutine finished" << endl;
}

int main() {
    auto task = coroutine_example();
    cout << "After creating coroutine" << endl;
    task.resume();
    cout << "After first resume" << endl;
    task.resume();
    cout << "After second resume" << endl;
    
    return 0;
}
```

## 五、内存管理

### 1. 内存分配与释放

```cpp
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
    unique_ptr<int> up(new int(40));
    cout << "*up = " << *up << endl;
    // 自动释放
    
    shared_ptr<int> sp(new int(50));
    cout << "*sp = " << *sp << endl;
    // 自动释放
    
    return 0;
}
```

### 2. 内存对齐

```cpp
#include <iostream>
#include <cstdalign>
using namespace std;

struct alignas(16) AlignedStruct {
    char c;
    int i;
    double d;
};

int main() {
    cout << "Size of char: " << sizeof(char) << endl;
    cout << "Size of int: " << sizeof(int) << endl;
    cout << "Size of double: " << sizeof(double) << endl;
    cout << "Size of AlignedStruct: " << sizeof(AlignedStruct) << endl;
    cout << "Alignment of AlignedStruct: " << alignof(AlignedStruct) << endl;
    
    return 0;
}
```

### 3. 内存池

```cpp
#include <iostream>
#include <vector>
#include <cstddef>
using namespace std;

class MemoryPool {
private:
    vector<void*> blocks;
    size_t blockSize;
    size_t blocksPerChunk;
    char* currentBlock;
    size_t currentPosition;

public:
    MemoryPool(size_t size, size_t perChunk = 1024) : blockSize(size), blocksPerChunk(perChunk) {
        allocateChunk();
    }
    
    ~MemoryPool() {
        for (void* block : blocks) {
            delete[] static_cast<char*>(block);
        }
    }
    
    void* allocate() {
        if (currentPosition + blockSize > blocksPerChunk * blockSize) {
            allocateChunk();
        }
        void* result = currentBlock + currentPosition;
        currentPosition += blockSize;
        return result;
    }
    
    void deallocate(void* ptr) {
        // 简单实现，不做任何操作
        // 实际应用中可能需要更复杂的管理
    }

private:
    void allocateChunk() {
        char* newBlock = new char[blocksPerChunk * blockSize];
        blocks.push_back(newBlock);
        currentBlock = newBlock;
        currentPosition = 0;
    }
};

// 测试代码
class TestClass {
private:
    int value;

public:
    TestClass(int v) : value(v) {
        cout << "TestClass constructed with value " << value << endl;
    }
    
    ~TestClass() {
        cout << "TestClass destructed with value " << value << endl;
    }
    
    int getValue() const {
        return value;
    }
};

int main() {
    MemoryPool pool(sizeof(TestClass));
    
    // 分配内存并构造对象
    TestClass* obj1 = new (pool.allocate()) TestClass(1);
    TestClass* obj2 = new (pool.allocate()) TestClass(2);
    TestClass* obj3 = new (pool.allocate()) TestClass(3);
    
    cout << "obj1 value: " << obj1->getValue() << endl;
    cout << "obj2 value: " << obj2->getValue() << endl;
    cout << "obj3 value: " << obj3->getValue() << endl;
    
    // 手动析构对象
    obj1->~TestClass();
    obj2->~TestClass();
    obj3->~TestClass();
    
    // 内存池会在析构时释放所有内存
    
    return 0;
}
```

## 六、并发编程

### 1. 线程

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

mutex mtx;
int counter = 0;

void increment(int id, int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(mtx);
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
```

### 2. 互斥量

```cpp
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
```

### 3. 条件变量

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

mutex mtx;
condition_variable cv;
queue<int> data_queue;
bool done = false;

void producer() {
    for (int i = 0; i < 10; i++) {
        { 
            lock_guard<mutex> lock(mtx);
            data_queue.push(i);
            cout << "Produced: " << i << endl;
        }
        cv.notify_one(); // 通知消费者
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    { 
        lock_guard<mutex> lock(mtx);
        done = true;
    }
    cv.notify_one(); // 通知消费者
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return !data_queue.empty() || done; });
        
        if (done && data_queue.empty()) {
            break;
        }
        
        int value = data_queue.front();
        data_queue.pop();
        cout << "Consumed: " << value << endl;
    }
}

int main() {
    thread producer_thread(producer);
    thread consumer_thread(consumer);
    
    producer_thread.join();
    consumer_thread.join();
    
    cout << "Done" << endl;
    
    return 0;
}
```

### 4. 原子操作

```cpp
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
```

### 5. 异步任务

```cpp
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
```

## 七、性能优化

### 1. 编译器优化

1. **优化级别**：
   - O0：无优化
   - O1：基本优化
   - O2：更多优化
   - O3：最高优化
   - Os：优化代码大小

2. **编译选项**：
   - `-march=native`：使用本地架构优化
   - `-mtune=native`：针对本地架构调整
   - `-ffast-math`：启用快速数学运算
   - `-flto`：链接时优化

3. **示例**：
   ```bash
   g++ -O3 -march=native -mtune=native -ffast-math -flto main.cpp -o main
   ```

### 2. 算法优化

1. **时间复杂度优化**：
   - 选择合适的算法（如排序算法）
   - 减少时间复杂度（如O(n²) → O(n log n)）

2. **空间复杂度优化**：
   - 减少内存使用
   - 使用原地算法

3. **示例**：
   ```cpp
   // 优化前：O(n²) 的冒泡排序
   void bubbleSort(vector<int>& arr) {
       int n = arr.size();
       for (int i = 0; i < n-1; i++) {
           for (int j = 0; j < n-i-1; j++) {
               if (arr[j] > arr[j+1]) {
                   swap(arr[j], arr[j+1]);
               }
           }
       }
   }
   
   // 优化后：O(n log n) 的快速排序
   void quickSort(vector<int>& arr, int low, int high) {
       if (low < high) {
           int pivot = arr[high];
           int i = low - 1;
           for (int j = low; j < high; j++) {
               if (arr[j] <= pivot) {
                   i++;
                   swap(arr[i], arr[j]);
               }
           }
           swap(arr[i+1], arr[high]);
           int pi = i+1;
           quickSort(arr, low, pi-1);
           quickSort(arr, pi+1, high);
       }
   }
   ```

### 3. 内存优化

1. **减少内存分配**：
   - 使用对象池
   - 预分配内存
   - 避免频繁的内存分配和释放

2. **缓存优化**：
   - 数据局部性
   - 减少缓存未命中
   - 使用适当的数据结构

3. **示例**：
   ```cpp
   // 优化前：频繁的内存分配
   vector<int> process(int n) {
       vector<int> result;
       for (int i = 0; i < n; i++) {
           result.push_back(i * 2); // 可能导致多次内存重分配
       }
       return result;
   }
   
   // 优化后：预分配内存
   vector<int> process_optimized(int n) {
       vector<int> result;
       result.reserve(n); // 预分配内存
       for (int i = 0; i < n; i++) {
           result.push_back(i * 2); // 避免内存重分配
       }
       return result;
   }
   ```

### 4. 并行优化

1. **多线程**：
   - 使用std::thread
   - 使用线程池

2. **SIMD**：
   - 使用向量化指令
   - 编译器自动向量化

3. **示例**：
   ```cpp
   // 并行计算
   #include <execution>
   #include <algorithm>
   
   void parallel_sum(vector<int>& arr) {
       int sum = 0;
       std::for_each(std::execution::par, arr.begin(), arr.end(), [&sum](int x) {
           sum += x;
       });
       cout << "Sum: " << sum << endl;
   }
   ```

### 5. 性能分析工具

1. **gprof**：
   ```bash
   g++ -pg main.cpp -o main
   ./main
   gprof main gmon.out > analysis.txt
   ```

2. **Valgrind**：
   ```bash
   valgrind --tool=callgrind ./main
   kcachegrind callgrind.out
   ```

3. **Perf**：
   ```bash
   perf record ./main
   perf report
   ```

4. **Intel VTune**：商业性能分析工具

## 八、常见考点

### 1. 智能指针的区别

**问题**：请描述unique_ptr、shared_ptr和weak_ptr的区别。

**答案**：
- **unique_ptr**：独占所有权，不能复制，只能移动
- **shared_ptr**：共享所有权，使用引用计数
- **weak_ptr**：不增加引用计数，用于解决shared_ptr的循环引用问题

### 2. 移动语义

**问题**：请解释移动语义和右值引用的作用。

**答案**：
- **右值引用**：用于引用临时对象
- **移动语义**：通过移动构造函数和移动赋值运算符，避免不必要的复制操作，提高性能
- **作用**：减少内存分配和复制开销，特别是对于大对象

### 3. 内存管理

**问题**：请描述内存分配的几种方式及其区别。

**答案**：
- **静态内存**：在编译时分配，生命周期与程序相同
- **栈内存**：在函数调用时分配，函数返回时自动释放
- **堆内存**：在运行时动态分配，需要手动释放
- **区别**：分配方式、生命周期、大小限制、访问速度

### 4. 并发编程

**问题**：请描述线程安全的实现方法。

**答案**：
- **互斥量**：使用mutex保护共享资源
- **原子操作**：使用atomic类型
- **条件变量**：用于线程间通信
- **读写锁**：允许多个读操作同时进行
- **无锁数据结构**：使用原子操作实现

### 5. 性能优化

**问题**：请描述C++程序的性能优化策略。

**答案**：
- **编译器优化**：使用合适的优化级别
- **算法优化**：选择高效的算法
- **内存优化**：减少内存分配，提高缓存命中率
- **并行优化**：使用多线程和SIMD
- **性能分析**：使用性能分析工具找出瓶颈

## 九、总结

C++高级特性和性能优化是C++开发中的重要组成部分，掌握这些知识对于编写高性能、可维护的代码至关重要。通过本文档的学习，您应该能够：

1. **掌握C++11/14/17/20的新特性**：自动类型推导、智能指针、移动语义、lambda表达式等
2. **理解内存管理**：内存分配与释放、内存对齐、内存池
3. **掌握并发编程**：线程、互斥量、条件变量、原子操作、异步任务
4. **优化程序性能**：编译器优化、算法优化、内存优化、并行优化
5. **使用性能分析工具**：gprof、Valgrind、Perf等
6. **应对常见考点**：熟悉C++高级特性和性能优化面试中的高频问题

在准备校招笔试面试时，建议结合实际项目经验，加深对C++高级特性和性能优化的理解。通过系统的学习和实践，您一定能够在C++相关的面试中取得优异的成绩。