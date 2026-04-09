# C++基础语法写法大全（快速识别版）

本文聚焦C++基础语法核心写法，简洁全面，无多余讲解，重点覆盖“遇到能认识”的基础知识点，适配Java转C++开发者快速上手识别。

# 一、核心关键字（必认）

## 1. 基础关键字

- 数据类型相关：int、char、bool、float、double、long、short、void、wchar_t

- 修饰符相关：const（常量）、static（静态）、unsigned（无符号）、signed（有符号）、volatile（易变）

- 控制流相关：if、else、for、while、do、switch、case、break、continue、default、return

- 内存相关：new（申请堆内存）、delete（释放堆内存）、malloc、free（兼容C）

- 面向对象相关：class（类）、struct（结构体）、public（公有）、private（私有）、protected（保护）、virtual（虚函数）、friend（友元）、this（当前对象指针）

- 泛型/模板相关：template（模板）、typename（模板类型）、class（模板类型，同typename）

- 其他核心：namespace（命名空间）、using（使用命名空间/别名）、enum（枚举）、typedef（类型别名）、inline（内联函数）

# 二、数据类型与变量声明

## 1. 基本数据类型声明

```cpp
// 基本类型
int a = 10;          // 整型
char c = 'A';        // 字符型（1字节）
bool b = true;       // 布尔型（true=1，false=0）
float f = 3.14f;     // 单精度浮点型（加f区分double）
double d = 3.14159;  // 双精度浮点型
long l = 100000L;    // 长整型（加L区分int）
long long ll = 1e18; // 长长整型（C++11+）

// 修饰符组合
const int num = 100;     // 常量（不可修改）
unsigned int u = 200;    // 无符号整型（无负数）
static int s = 5;        // 静态变量（生命周期随程序）

```

## 2. 类型别名（typedef/using）

```cpp
typedef int MyInt;       // 给int起别名MyInt
using MyDouble = double; // C++11+ 写法，更简洁
MyInt x = 10;
MyDouble y = 3.14;

```

## 3. 枚举（enum）

```cpp
// 普通枚举
enum Color { RED, GREEN, BLUE }; // 默认值 0,1,2
Color color = RED;

// 强类型枚举（C++11+，推荐）
enum class Fruit { APPLE, BANANA, ORANGE };
Fruit fruit = Fruit::APPLE;

```

# 三、数组（声明+使用）

## 1. 栈数组（最常用，自动释放）

```cpp
// 声明+初始化
int arr1[5];          // 未初始化（值随机）
int arr2[5] = {1,2,3};// 部分初始化，剩余为0
int arr3[] = {1,2,3,4,5};// 自动推断长度
char arr4[] = "hello";// 字符串数组（自动加'\0'结束符）

// 使用（下标访问，无越界检查）
arr2[0] = 10;         // 赋值
cout << arr3[2];      // 访问下标2，输出3
cout << sizeof(arr3)/sizeof(arr3[0]); // 计算数组长度

```

## 2. 堆数组（手动申请/释放）

```cpp
// 声明（new申请，返回指针）
int* arr = new int[5];     // 未初始化
int* arr2 = new int[5]();  // 初始化所有元素为0

// 使用（指针访问）
arr[1] = 20;
cout << arr2[3];

// 必须手动释放（避免内存泄漏）
delete[] arr;  // 数组释放要加[]
delete[] arr2;
arr = nullptr; // 置空，避免悬空指针

```

# 四、函数（声明+定义+调用）

```cpp
// 1. 声明（头文件中常用）
int add(int a, int b); // 声明，无实现
void print();          // 无返回值，无参数

// 2. 定义（源文件中实现）
int add(int a, int b) {
    return a + b;      // 返回值
}

void print() {
    cout << "Hello C++";
}

// 3. 调用
int sum = add(3, 5);
print();

// 4. 特殊函数
inline int max(int a, int b) { // 内联函数（减少函数调用开销）
    return a > b ? a : b;
}

static void func() { // 静态函数（仅当前文件可见）
    // 实现
}

```

# 五、类与对象（创建+使用）

## 1. 类的声明与定义

```cpp
class Person { // 类定义，末尾必须加分号
private: // 私有成员（仅类内访问）
    string name;
    int age;
public: // 公有成员（外部可访问）
    // 构造函数（创建对象时调用）
    Person() {} // 无参构造
    Person(string n, int a) : name(n), age(a) {} // 初始化列表（推荐）
    
    // 成员方法
    void setName(string n) { name = n; }
    string getName() { return name; }
    
    // 析构函数（对象销毁时调用，释放资源）
    ~Person() {}
}; // 必须加分号！

```

## 2. 对象创建与使用（两种方式）

```cpp
// 1. 栈对象（自动销毁，无需手动管理）
Person p1;                  // 调用无参构造
Person p2("张三", 20);      // 调用有参构造
p2.setName("李四");         // 调用成员方法
cout << p2.getName();       // 访问成员方法

// 2. 堆对象（手动申请/释放）
Person* p3 = new Person("王五", 25); // new创建，返回指针
p3->setName("赵六");        // 指针访问成员，用->
cout << p3->getName();
delete p3;                  // 手动释放
p3 = nullptr;               // 置空

```

## 3. 结构体（struct，与class差异：默认public）

```cpp
struct Student { // 结构体，默认公有
    string name;
    int score;
};

Student s = {"小明", 90}; // 直接初始化
cout << s.name;

```

# 六、继承（类的继承写法）

```cpp
// 父类（基类）
class Person {
protected: // 保护成员（子类可访问）
    string name;
public:
    Person(string n) : name(n) {}
    virtual void say() { // 虚函数（支持多态）
        cout << "我是人";
    }
};

// 子类（派生类），继承权限：public/protected/private
class Student : public Person { // public继承（最常用）
private:
    int score;
public:
    // 子类构造函数，必须调用父类构造
    Student(string n, int s) : Person(n), score(s) {}
    
    // 重写父类虚函数（多态）
    void say() override { // override（C++11+，检查重写）
        cout << "我是学生，名字：" << name;
    }
};

// 多继承（C++独有，可继承多个父类）
class Teacher : public Person {
    // 实现
};
class Graduate : public Student, public Teacher { // 多继承
    // 实现
};

```

# 七、泛型（模板，C++泛型核心）

## 1. 函数模板（通用函数）

```cpp
// 模板声明：template <typename 类型名>
template <typename T>
T swap(T& a, T& b) { // 通用交换函数，支持任意类型
    T temp = a;
    a = b;
    b = temp;
    return a;
}

// 调用（自动推导类型）
int a=1, b=2;
swap(a, b); // T推导为int
string s1="hello", s2="world";
swap(s1, s2); // T推导为string

```

## 2. 类模板（通用类）

```cpp
template <typename T> // T为模板类型参数
class MyContainer {
private:
    T data;
public:
    MyContainer(T d) : data(d) {}
    T getData() { return data; }
};

// 调用（必须指定类型）
MyContainer<int> c1(100);      // T=int
MyContainer<string> c2("test"); // T=string
cout << c1.getData();

```

# 八、输入输出（基础写法）

```cpp
#include <iostream> // 必须引入头文件
using namespace std; // 避免写std::

// 输出
cout << "Hello" << endl; // 换行输出
cout << 100 << " " << 3.14 << endl; // 多值输出

// 输入
int a;
string s;
cin >> a; // 读取int（遇空格/换行停止）
cin >> s; // 读取string（遇空格/换行停止）
getline(cin, s); // 读取整行（包含空格）

```

# 九、控制流（基础写法）

```cpp
// 1. if-else
if (a > 10) {
    cout << "大于10";
} else if (a == 10) {
    cout << "等于10";
} else {
    cout << "小于10";
}

// 2. for循环
for (int i=0; i<5; i++) {
    cout << i;
}
// 范围for（C++11+）
int arr[] = {1,2,3};
for (int x : arr) {
    cout << x;
}

// 3. while/do-while
while (a > 0) {
    a--;
}
do {
    a--;
} while (a > 0);

// 4. switch-case
switch (a) {
    case 1:
        cout << "1";
        break; // 必须加break，否则穿透
    case 2:
        cout << "2";
        break;
    default:
        cout << "其他";
}

```

# 十、指针与引用（基础写法）

## 1. 指针（*）

```cpp
int a = 10;
int* p = &a; // p是指针，存储a的地址（&取地址）
cout << p;  // 输出地址
cout << *p; // 解引用，输出a的值（10）

int* p2 = nullptr; // 空指针（C++11+，推荐）

```

## 2. 引用（&）

```cpp
int a = 10;
int& b = a; // b是a的别名，必须初始化，不可改指向
b = 20;     // 修改b，等价于修改a
cout << a;  // 输出20

```

# 十一、STL容器基础
```cpp
#include <vector>    // 动态数组（核心）
#include <string>    // 字符串（STL封装）
#include <unordered_map> // 哈希表（无序）
#include <unordered_set> // 哈希集合（无序）
#include <map>       // 有序哈希表（红黑树）
#include <set>       // 有序集合（红黑树）
#include <queue>     // 队列
#include <stack>     // 栈

// 1. vector（动态数组，核心详解）
vector<int> vec;               // 空vector（底层是连续内存，初始容量0）
vec.push_back(1);              // 尾部添加元素（容量不足时自动扩容，复制旧数据）
vec.emplace_back(2);           // C++11+，直接构造元素（比push_back更高效）
vec.resize(5);                 // 调整大小为5，新增位置填0
vec.reserve(10);               // 预分配容量（避免频繁扩容）
cout << vec.at(0);             // 访问下标0（有越界检查，比[]安全）
vec.erase(vec.begin() + 1);    // 删除第2个元素
vec.clear();                   // 清空元素（容量不变）
cout << vec.capacity();        // 实际分配的内存容量
cout << vec.size();            // 实际元素个数

// 2. string（补充详解）
string s = "hello";
s.append(" world");            // 等价于s += "world"，拼接字符串
s.insert(2, "xx");             // 在索引2处插入"xx" → "hexxllo"
s.erase(0, 2);                 // 从索引0开始删除2个字符
s.replace(1, 3, "abc");        // 从索引1开始替换3个字符为"abc"
cout << s.substr(1, 3);        // 截取子串：从索引1开始，长度3
cout << s.find("ll");          // 查找子串，返回首次出现的索引（找不到返回string::npos）

// 3. unordered_map（哈希表，核心详解）
unordered_map<string, int> umap;
umap["apple"] = 5;             // 键不存在则插入，存在则修改值
auto iter = umap.find("apple");// 查找键，返回迭代器（找不到返回umap.end()）
if (iter != umap.end()) {
    cout << iter->first;       // 迭代器访问键（first=键）
    cout << iter->second;      // 迭代器访问值（second=值）
}
umap.erase("apple");           // 删除指定键的键值对
umap.clear();                  // 清空所有元素

// 4. unordered_set（哈希集合，无重复）
unordered_set<int> uset = {1,2,3};
uset.insert(4);                // 添加元素（重复添加会被忽略）
uset.erase(2);                 // 删除元素
if (uset.count(3)) {           // 检查元素是否存在（存在返回1，否则0）
    cout << "3存在";
}

// 5. queue（队列，先进先出）
queue<int> q;
q.push(1);                     // 尾部入队
q.pop();                       // 头部出队（不返回值）
cout << q.front();             // 访问队头
cout << q.back();              // 访问队尾

// 6. stack（栈，后进先出）
stack<int> st;
st.push(1);                    // 栈顶入栈
st.pop();                      // 栈顶出栈（不返回值）
cout << st.top();              // 访问栈顶
```

# 十二、类与对象（逐行拆解核心：构造函数/析构函数/成员）
## 1. 类的完整声明与逐行解释
```cpp
// 类的定义：class关键字 + 类名 + 花括号 + 分号（必须加分号！）
class Person { 
    // 访问权限修饰符：private/protected/public，不写默认private
private: // 私有成员：仅类内部能访问，外部（包括子类）都不能直接访问
    // 成员变量（属性）：存储对象的状态
    string name; // 字符串类型的姓名
    int age;     // 整型的年龄

protected: // 保护成员：类内部 + 子类能访问，外部不能访问
    string id;   // 身份证号（子类可继承访问）

public: // 公有成员：类内部、子类、外部都能访问（对外提供的接口）
    // 1. 无参构造函数：创建对象时自动调用，无返回值，名称和类名完全一致
    Person() { 
        // 空构造：如果不写，编译器会自动生成一个默认无参构造（空实现）
        // 作用：初始化对象的默认状态，比如给成员变量赋默认值
        name = "未知";
        age = 0;
    }

    // 2. 有参构造函数（带初始化列表）：推荐写法！
    // 格式：类名(参数列表) : 成员变量1(参数1), 成员变量2(参数2) { 函数体 }
    Person(string n, int a) : name(n), age(a) { 
        // 初始化列表：直接给成员变量赋值（比函数体内赋值更高效）
        // 原因：成员变量在进入函数体前就已创建，初始化列表是"创建时直接赋值"，函数体是"先创建（脏数据）再赋值"
        // 函数体：可写额外逻辑（比如参数校验）
        if (a < 0) {
            age = 0; // 校验年龄，避免负数
        }
    }

    // 3. 拷贝构造函数：用已有对象创建新对象时调用（C++特有）
    // 格式：类名(const 类名& 引用名)
    Person(const Person& p) { 
        // const：防止修改原对象；引用：避免拷贝对象（减少开销）
        name = p.name; // 拷贝原对象的姓名
        age = p.age;   // 拷贝原对象的年龄
    }

    // 4. 成员方法（行为）：操作对象的函数
    // 设置姓名：void无返回值，参数是string类型的n
    void setName(string n) { 
        name = n; // 给私有成员name赋值（外部不能直接改，通过公有方法控制）
    }
    // 获取姓名：返回string类型，无参数
    string getName() { 
        return name; // 对外暴露私有成员的值（只读，不允许修改）
    }

    // 5. 析构函数：对象销毁时自动调用（栈对象出作用域/堆对象delete时）
    // 格式：~类名()，无参数、无返回值，不能重载
    ~Person() { 
        // 作用：释放对象占用的资源（比如堆内存、文件句柄、网络连接）
        // 这里没有堆资源，所以空实现；如果有堆数组，要在这里delete[]
        cout << name << "对象已销毁";
    }

    // 6. 静态成员方法：属于类，不属于单个对象
    static int getCount() {
        // 静态方法只能访问静态成员变量，不能访问普通成员变量（因为没有this指针）
        return count;
    }

    // 静态成员变量：属于类，所有对象共享（必须在类外初始化）
    static int count;
}; // 类定义结束必须加分号！（容易忘，编译报错重点检查）

// 静态成员变量初始化：类名::变量名 = 初始值
int Person::count = 0;
```

## 2. 对象创建与使用（逐行解释）
```cpp
// 1. 栈对象：在栈内存创建，出作用域自动调用析构函数
Person p1;                  // 调用无参构造函数 → name="未知", age=0
Person p2("张三", 20);      // 调用有参构造 → name="张三", age=20
p2.setName("李四");         // 调用公有方法修改私有成员name → name="李四"
cout << p2.getName();       // 调用公有方法获取name → 输出"李四"
Person p3 = p2;             // 调用拷贝构造函数 → p3的name="李四", age=20

// 2. 堆对象：在堆内存创建，必须手动delete释放，否则内存泄漏
Person* p4 = new Person("王五", 25); // new申请堆内存 → 调用有参构造，返回指针
p4->setName("赵六");        // 指针访问成员方法：->（替代.，因为p4是指针）
cout << p4->getName();      // 访问p4的name → 输出"赵六"
delete p4;                  // 手动释放堆内存 → 调用析构函数，销毁p4
p4 = nullptr;               // 置空指针：避免p4成为"悬空指针"（指向已释放的内存）
```

## 3. 结构体（struct）与类的区别（逐行解释）
```cpp
struct Student { // struct关键字：默认访问权限是public（class默认private）
    string name; // 公有成员：外部可直接访问
    int score;   // 公有成员：外部可直接访问
}; // 同样要加分号！

// 结构体对象初始化：因为成员是public，可直接赋值
Student s = {"小明", 90}; // 聚合初始化（struct特有，class如果全public也能用）
cout << s.name;           // 直接访问公有成员 → 输出"小明"
s.score = 95;             // 直接修改公有成员 → score=95
```

# 十三、继承（逐行拆解多态/虚函数）
```cpp
// 父类（基类）
class Person {
protected: // 保护成员：子类可访问，外部不可访问
    string name;
public:
    // 父类构造函数：初始化name
    Person(string n) : name(n) {}
    
    // 虚函数：virtual关键字 + 返回值 + 函数名
    virtual void say() { 
        // 作用：支持多态（子类重写后，父类指针/引用指向子类对象时，调用子类的say()）
        // 如果不加virtual，就是普通函数，调用时看"指针/引用的类型"，不是"对象的实际类型"
        cout << "我是人，名字：" << name;
    }

    // 虚析构函数：必须加virtual！否则子类析构函数不会被调用（内存泄漏）
    virtual ~Person() {}
};

// 子类（派生类）：public继承（最常用）
// 格式：class 子类名 : 继承权限 父类名
class Student : public Person { 
private:
    int score; // 子类独有的成员变量
public:
    // 子类构造函数：必须先调用父类构造函数（初始化列表）
    // 解释：子类对象创建时，先构造父类部分，再构造子类部分
    Student(string n, int s) : Person(n), score(s) {}
    
    // 重写父类虚函数：override关键字（C++11+）
    void say() override { 
        // override：检查是否真的重写了父类的虚函数（参数/返回值必须完全一致，否则编译报错）
        // 作用：实现多态，子类对象的say()会覆盖父类的say()
        cout << "我是学生，名字：" << name << "，分数：" << score;
    }
};

// 多态使用（核心场景）
int main() {
    // 父类指针指向子类对象（多态的核心写法）
    Person* p = new Student("张三", 90);
    p->say(); // 调用子类的say() → 输出"我是学生，名字：张三，分数：90"
    delete p; // 调用子类析构 + 父类析构（因为父类析构是virtual）
    return 0;
}
```

# 十四、指针与引用（逐行拆解核心区别）
```cpp
// 1. 指针（*）：存储变量的内存地址，可空、可改指向
int a = 10;          // 定义整型变量a，值为10，存在栈内存
int* p = &a;         // &a：取a的内存地址；p是指针变量，存储a的地址
cout << p;           // 输出a的地址（比如0x7ffeefbff5c4）
cout << *p;          // *p：解引用，访问指针指向的变量 → 输出10
*p = 20;             // 修改指针指向的变量值 → a=20
p = nullptr;         // 指针置空（C++11+），表示不指向任何内存

int* p2 = new int(30); // new int(30)：在堆内存创建int，值为30，返回地址
delete p2;            // 释放堆内存，必须手动调用
p2 = nullptr;

// 2. 引用（&）：变量的"别名"，不可空、不可改指向，必须初始化
int b = 10;
int& rb = b;         // rb是b的别名，必须初始化（不能写int& rb;）
rb = 20;             // 修改rb等价于修改b → b=20
cout << b;           // 输出20
// int& rb2 = nullptr; // 错误：引用不能指向空
// rb = c;             // 不是改指向，是把c的值赋给rb（即b=c）

// 指针vs引用核心区别：
// - 指针：独立变量，存地址，可空、可改指向，需要解引用访问值
// - 引用：只是别名，和原变量共用地址，不可空、不可改指向，直接用（无需解引用）
```

# 十五、内存管理（new/delete vs malloc/free）
```cpp
// 1. new/delete（C++专属，推荐）
int* p1 = new int;       // 堆内存创建int，未初始化（值随机）
int* p2 = new int(10);   // 堆内存创建int，初始化为10
int* arr = new int[5](); // 堆数组，初始化所有元素为0（()表示值初始化）

delete p1;               // 释放单个变量
delete p2;               // 释放单个变量
delete[] arr;            // 释放数组必须加[]（否则只释放第一个元素，内存泄漏）

// 2. malloc/free（C语言，C++兼容）
int* p3 = (int*)malloc(sizeof(int)); // 申请内存，返回void*，必须强制类型转换
if (p3 == nullptr) {     // 检查是否申请成功（new失败会抛异常）
    cout << "申请失败";
}
*p3 = 20;                // 赋值
free(p3);                // 释放内存（无需加[]，数组也直接free）

// new/delete vs malloc/free核心区别：
// - new：自动调用构造函数，delete自动调用析构函数
// - malloc：只申请内存，不初始化，free只释放内存，不调用析构
// - new失败抛异常，malloc失败返回nullptr
```

# 十六、核心语法易错点（逐行标注）
```cpp
// 1. 数组越界：编译器不检查，运行时可能崩溃
int arr[5] = {1,2,3};
cout << arr[10]; // 越界访问（无报错，但运行时可能崩溃）

// 2. 悬空指针：指向已释放的内存
int* p = new int(10);
delete p;
cout << *p; // 悬空指针访问（未定义行为，可能输出随机值/崩溃）

// 3. 类定义末尾忘加分号
class A { // 错误示例
    int a;
} // 少分号！编译报错：error: expected ';' after class definition

// 4. 堆数组释放忘加[]
int* arr2 = new int[5];
delete arr2; // 错误：应该delete[] arr2，否则内存泄漏

// 5. 虚函数重写参数不一致（override会报错）
class B {
    virtual void func(int a) {}
};
class C : public B {
    void func(double a) override {} // 错误：参数类型不一致，override检查报错
};
```

# 核心总结（重点记忆）
1. **构造函数**：和类名同名，无返回值，创建对象时调用；初始化列表比函数体赋值更高效，拷贝构造用于对象拷贝。
2. **析构函数**：~类名()，对象销毁时调用，堆对象必须delete才会调用，父类析构要加virtual。
3. **访问权限**：class默认private，struct默认public；private仅类内访问，protected子类可访问，public外部可访问。
4. **指针vs引用**：指针是地址变量（可空、可改指向），引用是别名（不可空、不可改指向）。
5. **多态**：父类虚函数+子类override+父类指针/引用指向子类对象，核心是"运行时确定调用哪个函数"。
6. **内存管理**：栈对象自动销毁，堆对象（new/malloc）必须手动释放，数组释放要加[]。