# C++ 性能优化手册

本文档提供 C++ 性能优化的全面指南，涵盖内存管理、算法优化、编译优化等核心内容，帮助您编写更高效的 C++ 代码。

## 一、内存管理优化

### 1. 内存分配策略

#### 1.1 栈 vs 堆

| 内存类型 | 分配速度 | 生命周期 | 大小限制 | 适用场景 |
|---------|---------|---------|---------|----------|
| 栈 | 极快 | 自动 | 有限（几MB） | 局部变量、小对象 |
| 堆 | 较慢 | 手动 | 大（系统内存） | 大对象、长生命周期 |

**最佳实践**：
- 优先使用栈内存，仅在需要时使用堆内存
- 对于频繁创建/销毁的对象，考虑对象池

#### 1.2 内存池

**原理**：预先分配一块内存，用于频繁创建/销毁的对象，避免频繁的 `new/delete` 开销。

**实现示例**：

```cpp
class ObjectPool {
private:
    std::vector<Object*> objects;
    std::vector<Object*> free_objects;
public:
    Object* acquire() {
        if (!free_objects.empty()) {
            Object* obj = free_objects.back();
            free_objects.pop_back();
            return obj;
        } else {
            Object* obj = new Object();
            objects.push_back(obj);
            return obj;
        }
    }
    
    void release(Object* obj) {
        free_objects.push_back(obj);
    }
    
    ~ObjectPool() {
        for (Object* obj : objects) {
            delete obj;
        }
    }
};
```

### 2. 内存使用优化

#### 2.1 避免内存泄漏

**常见原因**：
- 未释放 `new` 分配的内存
- 循环引用（`shared_ptr`）
- 异常导致的资源未释放

**检测工具**：
- Valgrind（Linux）
- AddressSanitizer（GCC/Clang）
- Visual Leak Detector（Windows）

#### 2.2 内存对齐

**原理**：处理器按字长对齐访问内存，未对齐的访问会降低性能。

**最佳实践**：
- 合理安排结构体成员顺序，减少内存空洞
- 使用 `alignas` 关键字指定对齐方式
- 避免在热路径中使用未对齐的数据

**示例**：

```cpp
// 优化前（有内存空洞）
struct Bad {
    char c;    // 1字节
    double d;  // 8字节（有7字节空洞）
    int i;     // 4字节
};

// 优化后（无内存空洞）
struct Good {
    double d;  // 8字节
    int i;     // 4字节
    char c;    // 1字节
};
```

#### 2.3 缓存友好性

**原理**：处理器有多级缓存，访问缓存中的数据比主存快得多。

**最佳实践**：
- 数据局部性：按顺序访问数据
- 避免伪共享：避免多线程同时修改同一缓存行的数据
- 合理使用数组而非链表：数组连续存储，缓存命中率高

**示例**：

```cpp
// 缓存友好：顺序访问
for (int i = 0; i < size; i++) {
    sum += arr[i];
}

// 缓存不友好：随机访问
for (int i = 0; i < size; i++) {
    sum += arr[rand() % size];
}
```

## 二、算法优化

### 1. 时间复杂度分析

| 算法 | 时间复杂度 | 适用场景 |
|------|-----------|----------|
| 线性查找 | O(n) | 小数据集 |
| 二分查找 | O(log n) | 有序数据集 |
| 冒泡排序 | O(n²) | 小数据集 |
| 快速排序 | O(n log n) | 大多数场景 |
| 归并排序 | O(n log n) | 稳定排序 |
| 堆排序 | O(n log n) | 大顶堆/小顶堆 |

### 2. 算法选择

**根据数据规模选择**：
- 小数据（n < 100）：简单算法（冒泡排序）
- 中等数据（n < 10^4）：快速排序、归并排序
- 大数据（n > 10^6）：外部排序、分布式排序

**根据数据特性选择**：
- 几乎有序：插入排序
- 随机数据：快速排序
- 有重复元素：三路快速排序
- 稳定排序：归并排序

### 3. 常用优化技巧

#### 3.1 循环优化

- **循环展开**：减少循环开销
- **循环合并**：减少循环次数
- **循环不变式外提**：将循环内不变的计算移到循环外
- **向量化**：使用 SIMD 指令并行处理

**示例**：

```cpp
// 循环展开
for (int i = 0; i < size; i += 4) {
    sum += arr[i] + arr[i+1] + arr[i+2] + arr[i+3];
}

// 循环不变式外提
const int factor = 2 * PI;
for (int i = 0; i < size; i++) {
    result[i] = value * factor; // 避免每次计算 factor
}
```

#### 3.2 空间换时间

- **缓存**：使用哈希表缓存计算结果
- **预处理**：预先计算并存储结果
- **查表**：使用查找表替代复杂计算

**示例**：

```cpp
// 缓存斐波那契数列
std::unordered_map<int, int> fib_cache;

int fib(int n) {
    if (n <= 1) return n;
    if (fib_cache.count(n)) return fib_cache[n];
    int result = fib(n-1) + fib(n-2);
    fib_cache[n] = result;
    return result;
}
```

#### 3.3 减少函数调用开销

- **内联函数**：使用 `inline` 关键字
- **模板特化**：避免运行时多态
- **lambda 表达式**：减少函数对象的开销

**示例**：

```cpp
// 内联函数
inline int max(int a, int b) {
    return a > b ? a : b;
}

// 使用 lambda 避免函数对象开销
std::sort(vec.begin(), vec.end(), [](int a, int b) {
    return a < b;
});
```

## 三、编译优化

### 1. 编译器优化选项

#### 1.1 GCC/Clang 优化级别

| 选项 | 优化级别 | 效果 |
|------|---------|------|
| `-O0` | 无优化 | 调试用 |
| `-O1` | 基本优化 | 平衡速度和大小 |
| `-O2` | 进一步优化 | 常用生产级别 |
| `-O3` | 最大优化 | 速度优先 |
| `-Os` | 优化大小 | 代码大小优先 |
| `-Ofast` | 激进优化 | 忽略标准，可能不安全 |

**最佳实践**：
- 调试：`-O0 -g`
- 生产：`-O2` 或 `-O3`
- 嵌入式：`-Os`

#### 1.2 其他编译选项

- `-march=native`：针对当前 CPU 架构优化
- `-mtune=native`：针对当前 CPU 架构调优
- `-ffast-math`：启用快速数学库（可能不精确）
- `-fvectorize`：启用自动向量化

### 2. 链接优化

- `-flto`：链接时优化（Link Time Optimization）
- `-fwhole-program`：整个程序优化
- `-static`：静态链接，减少运行时开销

### 3. 编译器提示

- `__attribute__((hot))`：标记热点函数
- `__attribute__((cold))`：标记冷函数
- `__attribute__((always_inline))`：强制内联
- `__attribute__((noinline))`：禁止内联

**示例**：

```cpp
// 标记热点函数
__attribute__((hot))
void hot_function() {
    // 频繁调用的代码
}

// 强制内联
__attribute__((always_inline))
inline void critical_function() {
    // 性能关键代码
}
```

## 四、并行编程

### 1. 多线程

#### 1.1 `std::thread` 基本使用

```cpp
#include <thread>

void worker(int id) {
    // 线程工作
    std::cout << "Thread " << id << " working" << std::endl;
}

int main() {
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    
    t1.join();
    t2.join();
    
    return 0;
}
```

#### 1.2 线程池

**原理**：预先创建线程，避免频繁创建/销毁线程的开销。

**实现示例**：

```cpp
class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; i++) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { 
                            return this->stop || !this->tasks.empty(); 
                        });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    
    template<class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }
    
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }
};
```

### 2. 并发容器

- `std::atomic`：原子操作
- `std::mutex`：互斥锁
- `std::condition_variable`：条件变量
- `std::future`/`std::promise`：异步任务
- `std::shared_mutex`：读写锁

**示例**：

```cpp
#include <atomic>
#include <mutex>
#include <future>

// 原子变量
std::atomic<int> counter(0);

// 互斥锁
std::mutex mtx;
std::vector<int> shared_data;

// 异步任务
std::future<int> result = std::async(std::launch::async, [] {
    return 42;
});
int value = result.get();
```

### 3. 并行算法

- `std::execution::par`：并行执行
- `std::execution::seq`：顺序执行
- `std::execution::par_unseq`：并行且向量化执行

**示例**：

```cpp
#include <algorithm>
#include <execution>

std::vector<int> vec = {1, 2, 3, 4, 5};

// 并行排序
std::sort(std::execution::par, vec.begin(), vec.end());

// 并行转换
std::vector<int> result(vec.size());
std::transform(std::execution::par, vec.begin(), vec.end(), 
               result.begin(), [](int x) { return x * 2; });
```

## 五、性能分析工具

### 1. 性能分析器

| 工具 | 平台 | 特点 |
|------|------|------|
| Valgrind (Callgrind) | Linux | 详细的函数调用分析 |
| perf | Linux | 系统级性能分析 |
| gprof | Linux | 函数级性能分析 |
| VTune | Windows/Linux |  Intel 专用性能分析 |
| Instruments | macOS | 苹果专用性能分析 |
| Visual Studio Profiler | Windows | VS 集成性能分析 |

### 2. 使用 Valgrind

```bash
# 安装 Valgrind
sudo apt install valgrind

# 运行 Callgrind
valgrind --tool=callgrind ./app

# 分析结果
kcachegrind callgrind.out.*
```

### 3. 使用 perf

```bash
# 安装 perf
sudo apt install linux-tools-common

# 记录性能数据
perf record ./app

# 查看报告
perf report
```

## 六、常见性能陷阱

### 1. 字符串操作

**陷阱**：频繁的字符串拼接会导致多次内存分配。

**解决方案**：
- 使用 `std::string::reserve` 预分配空间
- 使用 `std::ostringstream` 进行复杂拼接
- 避免在循环中进行字符串拼接

**示例**：

```cpp
// 优化前
std::string result;
for (int i = 0; i < 1000; i++) {
    result += std::to_string(i); // 频繁分配内存
}

// 优化后
std::string result;
result.reserve(1000 * 5); // 预分配空间
for (int i = 0; i < 1000; i++) {
    result += std::to_string(i);
}
```

### 2. 虚函数调用

**陷阱**：虚函数调用有运行时开销。

**解决方案**：
- 对于性能关键路径，避免使用虚函数
- 考虑使用模板或 CRTP（奇异递归模板模式）
- 使用 `final` 关键字阻止虚函数重写

**示例**：

```cpp
// 虚函数（有开销）
class Base {
public:
    virtual void process() { /* ... */ }
};

// 模板替代（无开销）
template <typename Derived>
class Base {
public:
    void process() {
        static_cast<Derived*>(this)->do_process();
    }
};

class Derived : public Base<Derived> {
public:
    void do_process() { /* ... */ }
};
```

### 3. 过度使用动态分配

**陷阱**：频繁的 `new/delete` 会导致内存碎片和性能下降。

**解决方案**：
- 使用对象池
- 使用智能指针管理内存
- 优先使用栈内存

### 4. 不必要的拷贝

**陷阱**：频繁的对象拷贝会增加内存和 CPU 开销。

**解决方案**：
- 使用移动语义（`std::move`）
- 使用引用传递（`const&`）
- 避免返回大型对象的拷贝

**示例**：

```cpp
// 优化前
std::vector<int> create_vector() {
    std::vector<int> v(1000);
    return v; // 可能拷贝（C++11 前）
}

// 优化后（C++11+ 自动移动）
std::vector<int> create_vector() {
    std::vector<int> v(1000);
    return v; // 移动语义
}

// 传递引用
void process(const std::vector<int>& v) { /* ... */ }
```

## 七、性能优化流程

### 1. 性能分析

1. **确定瓶颈**：使用性能分析工具找到热点函数
2. **建立基准**：测量当前性能
3. **设定目标**：明确优化目标

### 2. 代码优化

1. **算法优化**：选择更高效的算法
2. **数据结构优化**：选择合适的数据结构
3. **内存优化**：减少内存分配和拷贝
4. **并行化**：利用多线程和 SIMD

### 3. 验证和测试

1. **性能测试**：验证优化效果
2. **功能测试**：确保功能正确性
3. **回归测试**：确保没有引入新问题

### 4. 持续优化

1. **监控**：在生产环境中监控性能
2. **迭代**：根据实际情况持续优化
3. **文档**：记录优化过程和结果

## 八、总结

C++ 性能优化是一个系统性工程，需要从多个角度入手：

1. **内存管理**：合理使用栈/堆，避免内存泄漏，提高缓存命中率
2. **算法优化**：选择合适的算法和数据结构，减少时间复杂度
3. **编译优化**：利用编译器优化选项，生成高效的机器码
4. **并行编程**：利用多线程和 SIMD，充分利用硬件资源
5. **性能分析**：使用专业工具，找出性能瓶颈

通过本文档的学习，您应该能够识别和解决 C++ 代码中的性能问题，编写更高效的代码。记住，性能优化是一个持续的过程，需要在实践中不断积累经验，平衡性能和代码可读性。