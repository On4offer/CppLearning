# C++ 代码示例与练习

本文档提供 C++ 常用代码示例和练习，涵盖设计模式、经典算法、小型项目实践等内容，帮助您通过实践巩固所学知识。

## 一、常用设计模式

### 1. 单例模式

**核心思想**：确保一个类只有一个实例，并提供全局访问点。

**实现示例**：

```cpp
class Singleton {
private:
    static Singleton* instance;
    
    // 私有构造函数
    Singleton() {
        std::cout << "Singleton created" << std::endl;
    }
    
    // 禁止拷贝
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
    
    void doSomething() {
        std::cout << "Doing something..." << std::endl;
    }
    
    ~Singleton() {
        std::cout << "Singleton destroyed" << std::endl;
    }
};

// 初始化静态成员
Singleton* Singleton::instance = nullptr;

// 使用
int main() {
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    
    std::cout << "s1 and s2 are the same: " << (s1 == s2) << std::endl;
    
    s1->doSomething();
    
    return 0;
}
```

### 2. 工厂模式

**核心思想**：将对象的创建与使用分离，通过工厂类创建对象。

**实现示例**：

```cpp
// 产品接口
class Product {
public:
    virtual void use() = 0;
    virtual ~Product() = default;
};

// 具体产品
class ConcreteProductA : public Product {
public:
    void use() override {
        std::cout << "Using Product A" << std::endl;
    }
};

class ConcreteProductB : public Product {
public:
    void use() override {
        std::cout << "Using Product B" << std::endl;
    }
};

// 工厂类
class Factory {
public:
    static Product* createProduct(const std::string& type) {
        if (type == "A") {
            return new ConcreteProductA();
        } else if (type == "B") {
            return new ConcreteProductB();
        }
        return nullptr;
    }
};

// 使用
int main() {
    Product* productA = Factory::createProduct("A");
    Product* productB = Factory::createProduct("B");
    
    productA->use();
    productB->use();
    
    delete productA;
    delete productB;
    
    return 0;
}
```

### 3. 观察者模式

**核心思想**：定义对象间的一对多依赖，当一个对象状态改变时，所有依赖它的对象都会收到通知。

**实现示例**：

```cpp
#include <vector>
#include <algorithm>

// 观察者接口
class Observer {
public:
    virtual void update(int value) = 0;
    virtual ~Observer() = default;
};

// 被观察者
class Subject {
private:
    std::vector<Observer*> observers;
    int state;
public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }
    
    void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void setState(int value) {
        state = value;
        notify();
    }
    
    int getState() {
        return state;
    }
    
    void notify() {
        for (Observer* observer : observers) {
            observer->update(state);
        }
    }
};

// 具体观察者
class ConcreteObserver : public Observer {
private:
    std::string name;
    Subject& subject;
public:
    ConcreteObserver(std::string name, Subject& subject) : name(name), subject(subject) {
        subject.attach(this);
    }
    
    void update(int value) override {
        std::cout << name << " received update: " << value << std::endl;
    }
};

// 使用
int main() {
    Subject subject;
    
    ConcreteObserver observer1("Observer 1", subject);
    ConcreteObserver observer2("Observer 2", subject);
    
    subject.setState(10);
    subject.setState(20);
    
    return 0;
}
```

## 二、经典算法实现

### 1. 排序算法

#### 1.1 快速排序

```cpp
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 使用
std::vector<int> arr = {10, 7, 8, 9, 1, 5};
quickSort(arr, 0, arr.size() - 1);
```

#### 1.2 归并排序

```cpp
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        
        merge(arr, left, mid, right);
    }
}

// 使用
std::vector<int> arr = {12, 11, 13, 5, 6, 7};
mergeSort(arr, 0, arr.size() - 1);
```

### 2. 搜索算法

#### 2.1 二分查找

```cpp
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1; // 未找到
}

// 使用
std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13};
int index = binarySearch(arr, 7); // 返回 3
```

#### 2.2 深度优先搜索 (DFS)

```cpp
#include <vector>
#include <stack>

void dfs(const std::vector<std::vector<int>>& graph, int start) {
    std::vector<bool> visited(graph.size(), false);
    std::stack<int> stack;
    
    stack.push(start);
    visited[start] = true;
    
    while (!stack.empty()) {
        int node = stack.top();
        stack.pop();
        
        std::cout << node << " " << std::endl;
        
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                stack.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

// 使用
std::vector<std::vector<int>> graph = {
    {1, 2},
    {0, 3, 4},
    {0, 5, 6},
    {1},
    {1},
    {2},
    {2}
};
dfs(graph, 0);
```

### 3. 动态规划

#### 3.1 斐波那契数列

```cpp
int fib(int n) {
    if (n <= 1) return n;
    
    std::vector<int> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;
    
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    
    return dp[n];
}

// 使用
int result = fib(10); // 返回 55
```

#### 3.2 最长公共子序列

```cpp
int longestCommonSubsequence(const std::string& text1, const std::string& text2) {
    int m = text1.size();
    int n = text2.size();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}

// 使用
std::string text1 = "abcde";
std::string text2 = "ace";
int result = longestCommonSubsequence(text1, text2); // 返回 3
```

## 三、小型项目实践

### 1. 命令行计算器

**功能**：支持基本算术运算（加、减、乘、除）。

**实现**：

```cpp
#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::cout << "简单计算器" << std::endl;
    std::cout << "输入格式：数字 运算符 数字" << std::endl;
    std::cout << "例如：1 + 2" << std::endl;
    
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "exit") break;
        
        std::istringstream iss(input);
        double num1, num2;
        char op;
        
        if (iss >> num1 >> op >> num2) {
            switch (op) {
                case '+':
                    std::cout << num1 + num2 << std::endl;
                    break;
                case '-':
                    std::cout << num1 - num2 << std::endl;
                    break;
                case '*':
                    std::cout << num1 * num2 << std::endl;
                    break;
                case '/':
                    if (num2 != 0) {
                        std::cout << num1 / num2 << std::endl;
                    } else {
                        std::cout << "错误：除数不能为零" << std::endl;
                    }
                    break;
                default:
                    std::cout << "错误：无效的运算符" << std::endl;
            }
        } else {
            std::cout << "错误：输入格式不正确" << std::endl;
        }
    }
    
    return 0;
}
```

### 2. 学生管理系统

**功能**：添加、删除、查询学生信息。

**实现**：

```cpp
#include <iostream>
#include <vector>
#include <string>

class Student {
private:
    std::string name;
    int id;
    double score;
public:
    Student(std::string name, int id, double score) : name(name), id(id), score(score) {}
    
    std::string getName() const { return name; }
    int getId() const { return id; }
    double getScore() const { return score; }
    
    void display() const {
        std::cout << "ID: " << id << ", 姓名: " << name << ", 分数: " << score << std::endl;
    }
};

class StudentManager {
private:
    std::vector<Student> students;
public:
    void addStudent() {
        std::string name;
        int id;
        double score;
        
        std::cout << "输入姓名: ";
        std::cin >> name;
        std::cout << "输入ID: ";
        std::cin >> id;
        std::cout << "输入分数: ";
        std::cin >> score;
        
        students.emplace_back(name, id, score);
        std::cout << "学生添加成功" << std::endl;
    }
    
    void removeStudent() {
        int id;
        std::cout << "输入要删除的学生ID: ";
        std::cin >> id;
        
        auto it = students.begin();
        while (it != students.end()) {
            if (it->getId() == id) {
                it = students.erase(it);
                std::cout << "学生删除成功" << std::endl;
                return;
            } else {
                ++it;
            }
        }
        
        std::cout << "未找到该学生" << std::endl;
    }
    
    void queryStudent() {
        int id;
        std::cout << "输入要查询的学生ID: ";
        std::cin >> id;
        
        for (const auto& student : students) {
            if (student.getId() == id) {
                student.display();
                return;
            }
        }
        
        std::cout << "未找到该学生" << std::endl;
    }
    
    void displayAll() {
        if (students.empty()) {
            std::cout << "没有学生信息" << std::endl;
            return;
        }
        
        std::cout << "所有学生信息:" << std::endl;
        for (const auto& student : students) {
            student.display();
        }
    }
};

int main() {
    StudentManager manager;
    int choice;
    
    while (true) {
        std::cout << "\n学生管理系统" << std::endl;
        std::cout << "1. 添加学生" << std::endl;
        std::cout << "2. 删除学生" << std::endl;
        std::cout << "3. 查询学生" << std::endl;
        std::cout << "4. 显示所有学生" << std::endl;
        std::cout << "5. 退出" << std::endl;
        std::cout << "请选择: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.removeStudent();
                break;
            case 3:
                manager.queryStudent();
                break;
            case 4:
                manager.displayAll();
                break;
            case 5:
                return 0;
            default:
                std::cout << "无效选择" << std::endl;
        }
    }
    
    return 0;
}
```

### 3. 文件读写工具

**功能**：读取文件内容，统计单词数量。

**实现**：

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

void countWords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }
    
    std::unordered_map<std::string, int> wordCount;
    std::string line, word;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        while (iss >> word) {
            // 简单处理：转换为小写，去除标点
            for (char& c : word) {
                if (std::ispunct(c)) {
                    c = ' ';
                } else {
                    c = std::tolower(c);
                }
            }
            
            // 重新提取单词
            std::istringstream wordIss(word);
            while (wordIss >> word) {
                wordCount[word]++;
            }
        }
    }
    
    file.close();
    
    std::cout << "单词统计结果:" << std::endl;
    for (const auto& pair : wordCount) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    std::cout << "总单词数: " << wordCount.size() << std::endl;
}

int main() {
    std::string filename;
    std::cout << "输入文件名: ";
    std::cin >> filename;
    
    countWords(filename);
    
    return 0;
}
```

## 四、常见面试题

### 1. 链表操作

#### 1.1 反转链表

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr != nullptr) {
        ListNode* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }
    
    return prev;
}

// 测试
ListNode* createList(const std::vector<int>& values) {
    ListNode* dummy = new ListNode(0);
    ListNode* current = dummy;
    
    for (int val : values) {
        current->next = new ListNode(val);
        current = current->next;
    }
    
    ListNode* head = dummy->next;
    delete dummy;
    return head;
}

void printList(ListNode* head) {
    while (head != nullptr) {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5};
    ListNode* head = createList(values);
    std::cout << "原链表: ";
    printList(head);
    
    ListNode* reversed = reverseList(head);
    std::cout << "反转后: ";
    printList(reversed);
    
    return 0;
}
```

#### 1.2 检测链表环

```cpp
bool hasCycle(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return false;
    }
    
    ListNode* slow = head;
    ListNode* fast = head->next;
    
    while (slow != fast) {
        if (fast == nullptr || fast->next == nullptr) {
            return false;
        }
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return true;
}
```

### 2. 树操作

#### 2.1 二叉树遍历

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 前序遍历
void preorderTraversal(TreeNode* root) {
    if (root == nullptr) return;
    
    std::cout << root->val << " ";
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

// 中序遍历
void inorderTraversal(TreeNode* root) {
    if (root == nullptr) return;
    
    inorderTraversal(root->left);
    std::cout << root->val << " ";
    inorderTraversal(root->right);
}

// 后序遍历
void postorderTraversal(TreeNode* root) {
    if (root == nullptr) return;
    
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    std::cout << root->val << " ";
}

// 测试
TreeNode* createTree() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    return root;
}

int main() {
    TreeNode* root = createTree();
    
    std::cout << "前序遍历: ";
    preorderTraversal(root);
    std::cout << std::endl;
    
    std::cout << "中序遍历: ";
    inorderTraversal(root);
    std::cout << std::endl;
    
    std::cout << "后序遍历: ";
    postorderTraversal(root);
    std::cout << std::endl;
    
    return 0;
}
```

### 3. 动态规划

#### 3.1 爬楼梯

```cpp
int climbStairs(int n) {
    if (n <= 2) return n;
    
    std::vector<int> dp(n + 1);
    dp[1] = 1;
    dp[2] = 2;
    
    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    
    return dp[n];
}

// 测试
int main() {
    int n = 5;
    std::cout << "爬" << n << "阶楼梯的方法数: " << climbStairs(n) << std::endl; // 输出 8
    return 0;
}
```

#### 3.2 最大子数组和

```cpp
int maxSubArray(const std::vector<int>& nums) {
    int maxCurrent = nums[0];
    int maxGlobal = nums[0];
    
    for (int i = 1; i < nums.size(); i++) {
        maxCurrent = std::max(nums[i], maxCurrent + nums[i]);
        if (maxCurrent > maxGlobal) {
            maxGlobal = maxCurrent;
        }
    }
    
    return maxGlobal;
}

// 测试
int main() {
    std::vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    std::cout << "最大子数组和: " << maxSubArray(nums) << std::endl; // 输出 6
    return 0;
}
```

## 五、练习题目

### 1. 基础练习

1. **实现一个函数**：计算两个数的最大公约数（GCD）。
2. **实现一个函数**：判断一个数是否为质数。
3. **实现一个函数**：反转字符串。
4. **实现一个函数**：检查字符串是否为回文。
5. **实现一个函数**：合并两个有序数组。

### 2. 进阶练习

1. **实现一个栈**：支持 push、pop、top 和 getMin 操作，且 getMin 操作的时间复杂度为 O(1)。
2. **实现一个队列**：使用两个栈实现。
3. **实现一个 LRU 缓存**：最近最少使用缓存。
4. **实现一个二叉搜索树**：支持插入、删除和查找操作。
5. **实现一个图**：支持深度优先搜索和广度优先搜索。

### 3. 项目练习

1. **实现一个简单的文本编辑器**：支持基本的编辑操作。
2. **实现一个简单的 HTTP 服务器**：处理 GET 请求。
3. **实现一个简单的数据库**：支持基本的 CRUD 操作。
4. **实现一个简单的游戏**：如贪吃蛇或俄罗斯方块。
5. **实现一个简单的图像处理工具**：如灰度转换、模糊等。

## 六、总结

通过本文档的代码示例和练习，您可以：

1. **掌握常用设计模式**：单例、工厂、观察者等模式的实现
2. **熟悉经典算法**：排序、搜索、动态规划等算法的实现
3. **实践小型项目**：命令行工具、学生管理系统、文件读写等
4. **准备面试**：常见链表、树、动态规划等面试题的解法

建议您：

- 先理解代码示例的逻辑
- 尝试自己实现，遇到问题再参考示例
- 逐步增加难度，从基础练习到项目练习
- 定期复习，巩固所学知识

通过不断练习，您的 C++ 编程能力将得到显著提升，为项目开发打下坚实的基础。