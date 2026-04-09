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
    // initializer_list 是一个模板类，表示一个元素类型为 T 的列表，这里 T 是 int
    // 作用是允许我们使用花括号 {} 来初始化 MyClass 对象，并将这些整数传递给构造函数，它是让类支持 {10,20,30} 这种写法
    // 和:初始化列表的区别是，initializer_list 是一个特殊的类型，专门用于接受花括号中的元素，而:初始化列表是构造函数的一部分，用于初始化成员变量
    MyClass(initializer_list<int> list) {
        for (auto num : list) {
            cout << "正在添加 " << num << endl;
            data.push_back(num);
        }
    }
    
    void print() {
        for (auto num : data) {
            // 输出 data 中的元素，data 是在构造函数中通过 initializer_list 初始化的成员变量
            cout << num << " ";
        }
        cout << endl;
    }
};

int main() {
    // 初始化 vector，系统会调用 vector 的 initializer_list 构造函数来创建 vector 对象，并将花括号中的整数传递给它
    cout << "初始化 vector:" << endl;
    vector<int> v = {1, 2, 3, 4, 5};
    cout << "初始化完成，vector 中的元素: ";
    for (auto num : v) {
        cout << num << " ";
    }
    cout << endl;

    
    // 初始化 map，系统会调用 map 的 initializer_list 构造函数来创建 map 对象，并将花括号中的键值对传递给它
    cout << "初始化 map:" << endl;
    map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    cout << "初始化完成，map 中的元素: " << endl;
    for (const auto& pair : m) {
        cout << "(" << pair.first << ": " << pair.second << ")" << endl;
    }

    // 初始化自定义类，MyClass 的构造函数接受一个 initializer_list<int>
    // 当我们使用花括号初始化 MyClass 对象时，系统会调用这个构造函数，并将花括号中的整数传递给它
    cout << "初始化自定义类:" << endl;
    MyClass obj = {10, 20, 30, 40, 50};
    cout << "自定义类初始化完成，自定义类中的元素: ";
    obj.print();
    
    return 0;
}