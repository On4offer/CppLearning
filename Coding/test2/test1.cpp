#include <iostream>
using namespace std;

class Person {
public:
    Person() { cout << "对象创建" << endl; }
    ~Person() { cout << "对象销毁" << endl; }
};

// 重载 << 运算符，使其能够输出 Person 对象
ostream& operator<<(ostream& os, const Person& p) {
    os << "Person 对象长这个样子"; // 简单输出对象描述
    return os;
}

int main() {
    // 方式1：栈对象（栈内存，自动销毁）
    Person p1; // 无需 new，直接创建，作用域是 main 函数
    // main 函数执行完，p1 自动销毁，调用析构函数
    cout << "p1 指向的内存地址：" << &p1 << endl;

    // 方式2：堆对象（堆内存，手动创建，必须手动销毁）
    Person* p2 = new Person(); // new 申请堆内存，返回指针
    cout << "p2 指向的内存地址：" << p2 << endl;
    cout << "p2 指向的对象：" << *p2 << endl;
    // 必须手动 delete，否则内存泄漏
    delete p2; // 释放堆内存，调用析构函数

    return 0;
}
