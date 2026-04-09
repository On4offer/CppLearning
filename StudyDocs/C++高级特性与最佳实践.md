# C++ 高级特性与最佳实践

本文档覆盖 C++ 核心高级特性和实际开发中的最佳实践，帮助从基础语法过渡到项目开发。

## 一、STL 深入

### 1. 容器的底层实现6

| 容器              | 底层实现   | 特点             | 时间复杂度                |
| --------------- | ------ | -------------- | -------------------- |
| `vector`        | 动态数组   | 随机访问快，尾部插入/删除快 | 访问 O(1)，尾部插入 O(1) 均摊 |
| `list`          | 双向链表   | 任意位置插入/删除快     | 插入/删除 O(1)，访问 O(n)   |
| `deque`         | 分段连续内存 | 两端插入/删除快       | 两端操作 O(1)，随机访问 O(1)  |
| `map`           | 红黑树    | 有序，自动排序        | 查找/插入/删除 O(log n)    |
| `unordered_map` | 哈希表    | 无序，查找快         | 平均 O(1)，最坏 O(n)      |
| `set`           | 红黑树    | 有序，无重复元素       | 查找/插入/删除 O(log n)    |
| `unordered_set` | 哈希表    | 无序，无重复元素       | 平均 O(1)，最坏 O(n)      |

### 2. 容器的选择策略

- **随机访问频繁**：选择 `vector`
- **频繁插入/删除**：选择 `list` 或 `deque`
- **键值对存储**：
  - 需要有序：`map`
  - 注重性能：`unordered_map`
- **去重**：
  - 需要有序：`set`
  - 注重性能：`unordered_set`

### 3. STL 算法

```cpp
#include <algorithm>

// 遍历
for_each(vec.begin(), vec.end(), [](int& num) { num *= 2; });

// 查找
auto it = find(vec.begin(), vec.end(), 10);

// 排序
sort(vec.begin(), vec.end());

// 计数
int count = count(vec.begin(), vec.end(), 5);

// 最大值/最小值
auto max_it = max_element(vec.begin(), vec.end());
auto min_it = min_element(vec.begin(), vec.end());

// 拷贝
vector<int> dest(vec.size());
copy(vec.begin(), vec.end(), dest.begin());

// 转换
vector<string> str_vec;
transform(vec.begin(), vec.end(), back_inserter(str_vec), 
    [](int num) { return to_string(num); });
```

## 二、智能指针

### 1. 智能指针的类型

| 智能指针         | 所有权 | 特点       | 使用场景     |
| ------------ | --- | -------- | -------- |
| `unique_ptr` | 独占  | 不可复制，可移动 | 单个所有者的资源 |
| `shared_ptr` | 共享  | 引用计数，可复制 | 多个所有者的资源 |
| `weak_ptr`   | 弱引用 | 不增加引用计数  | 避免循环引用   |

### 2. 智能指针的使用

```cpp
#include <memory>

// unique_ptr
std::unique_ptr<int> p1(new int(10));
std::unique_ptr<int> p2 = std::make_unique<int>(20); // C++14+
// std::unique_ptr<int> p3 = p1; // 错误：不可复制
std::unique_ptr<int> p4 = std::move(p1); // 正确：可移动

// shared_ptr
std::shared_ptr<int> sp1(new int(100));
std::shared_ptr<int> sp2 = std::make_shared<int>(200); // 推荐
std::shared_ptr<int> sp3 = sp1; // 正确：引用计数+1

// weak_ptr
std::weak_ptr<int> wp = sp1;
if (auto sp = wp.lock()) { // 检查是否有效
    std::cout << *sp << std::endl;
}
```

### 3. 最佳实践

- **优先使用** **`make_unique`** **和** **`make_shared`**：避免内存泄漏风险
- **避免使用** **`shared_ptr`** **循环引用**：使用 `weak_ptr` 打破循环
- **智能指针作为函数参数**：传递值或 const 引用
- **智能指针作为成员变量**：根据所有权选择合适类型

## 三、移动语义

### 1. 核心概念

- **左值**：可取地址的表达式（如变量名）
- **右值**：临时表达式（如字面量、临时对象）
- **移动构造函数**：`Class(Class&&)`，转移资源
- **移动赋值运算符**：`Class& operator=(Class&&)`，转移资源
- **`std::move`**：将左值转换为右值引用

### 2. 移动语义的使用

```cpp
class Person {
private:
    std::string name;
public:
    // 移动构造函数
    Person(Person&& p) noexcept : name(std::move(p.name)) {
        std::cout << "移动构造" << std::endl;
    }
    
    // 移动赋值运算符
    Person& operator=(Person&& p) noexcept {
        if (this != &p) {
            name = std::move(p.name);
            std::cout << "移动赋值" << std::endl;
        }
        return *this;
    }
};

// 使用
Person p1("张三");
Person p2 = std::move(p1); // 移动构造。move()强行把p1变成右值
Person p3;
p3 = std::move(p2); // 移动赋值
```

### 3. 最佳实践

- **为管理资源的类实现移动语义**：提高性能
- **标记移动操作** **`noexcept`**：允许标准库容器使用移动操作
- **合理使用** **`std::move`**：只对不再使用的对象使用
- **理解移动后原对象的状态**：有效但未定义

## 四、模板编程

### 1. 函数模板

```cpp
// 基本函数模板
template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 调用
int i = max(10, 20);
double d = max(3.14, 2.71);

// 显式指定类型
std::string s = max<std::string>("hello", "world");
```

### 2. 类模板

```cpp
// 类模板
template <typename T>
class Stack {
private:
    std::vector<T> elements;
public:
    void push(const T& item) {
        elements.push_back(item);
    }
    T pop() {
        T top = elements.back();
        elements.pop_back();
        return top;
    }
};

// 实例化
Stack<int> intStack;
Stack<std::string> stringStack;
```

### 3. 模板特化

```cpp
// 主模板
template <typename T>
class MyClass {
public:
    void print() {
        std::cout << "通用模板" << std::endl;
    }
};

// 特化版本
template <>
class MyClass<int> {
public:
    void print() {
        std::cout << "int 特化" << std::endl;
    }
};
```

### 4. 变参模板

```cpp
// 递归终止条件
template <typename T>
void print(const T& value) {
    std::cout << value << std::endl;
}

// 变参模板
template <typename T, typename... Args>
void print(const T& value, const Args&... args) {
    std::cout << value << " ";
    print(args...); // 递归调用
}

// 调用
print(1, 2.5, "hello");
```

## 五、异常处理

### 1. 异常的基本使用

```cpp
try {
    // 可能抛出异常的代码
    if (something_wrong) {
        throw std::runtime_error("发生错误");
    }
} catch (const std::exception& e) {
    // 捕获异常
    std::cerr << "异常: " << e.what() << std::endl;
} catch (...) {
    // 捕获所有其他异常
    std::cerr << "未知异常" << std::endl;
}
```

### 2. 自定义异常

```cpp
class MyException : public std::exception {
private:
    std::string message;
public:
    MyException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 抛出自定义异常
if (error) {
    throw MyException("自定义错误");
}
```

### 3. 异常安全

- **基本保证**：异常发生后，程序处于有效状态，但可能与之前不同
- **强保证**：异常发生后，程序状态与异常发生前完全相同
- **无抛出保证**：函数不会抛出异常

## 六、类型转换

| 转换类型               | 用途          | 安全性           |
| ------------------ | ----------- | ------------- |
| `static_cast`      | 静态类型转换      | 编译时检查，相对安全    |
| `dynamic_cast`     | 运行时类型转换（多态） | 运行时检查，安全      |
| `reinterpret_cast` | 位级别的转换      | 不安全，需谨慎使用     |
| `const_cast`       | 移除 const 限定 | 可能破坏 const 语义 |

### 示例

```cpp
// static_cast
int i = 10;
double d = static_cast<double>(i);

// dynamic_cast（用于多态）
class Base { virtual void foo() {} };
class Derived : public Base {};
Base* b = new Derived;
Derived* d = dynamic_cast<Derived*>(b); // 安全转换

// reinterpret_cast（谨慎使用）
int* p = new int(10);
long addr = reinterpret_cast<long>(p);

// const_cast
const int* cp = &i;
int* p = const_cast<int*>(cp); // 移除 const
```

## 七、最佳实践总结

1. **代码风格**：
   - 使用 `nullptr` 替代 `NULL`
   - 使用 `using` 替代 `typedef`（C++11+）
   - 避免使用 `using namespace std;`（大型项目）
2. **内存管理**：
   - 优先使用智能指针
   - 避免裸指针的手动管理
   - 注意内存对齐和缓存友好性
3. **性能优化**：
   - 减少不必要的拷贝（使用移动语义）
   - 合理使用 `const` 和 `constexpr`
   - 避免频繁的内存分配（使用 `reserve`）
4. **安全性**：
   - 使用 `explicit` 构造函数避免隐式转换
   - 注意异常安全和资源管理
   - 避免未定义行为（如数组越界）
5. **可维护性**：
   - 编写清晰的注释
   - 遵循命名规范
   - 模块化设计，避免过大的函数和类

通过掌握这些高级特性和最佳实践，您将能够编写更高效、更安全、更可维护的 C++ 代码，为项目开发打下坚实的基础。
