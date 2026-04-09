/*
工厂模式（Factory Pattern）是一种创建型设计模式，提供了一种创建对象的接口，
但由子类决定要实例化的类是哪一个。工厂模式使得一个类的实例化延迟到其子类。
工厂模式的核心思想是将对象的创建过程封装在一个工厂
*/
#include <iostream>
using namespace std;

// 产品接口
class Product {
    public:
    // virtual：虚函数，允许在派生类中重写
    // = 0：纯虚函数标记, 表示该函数没有实现，必须在派生类中实现
    // 纯虚函数使得 Product 成为一个抽象类，无法直接实例化，只能通过派生类来实现具体的功能。
    virtual void use() = 0;
    // virtual destructor: 确保通过基类指针删除派生类对象时，派生类的析构函数能够被正确调用，
    // 虚析构：保证多态安全释放避免资源泄漏。
    virtual ~Product() {
        std::cout << "Product destroyed" << std::endl;
    }
};

// 具体产品A
class ConcreteProductA : public Product {
    public:
    void use() override {
        std::cout << "Using ConcreteProductA" << std::endl;
    }
    ~ConcreteProductA() override {
        std::cout << "Destroying ConcreteProductA" << std::endl;
    }
};

// 具体产品B
class ConcreteProductB : public Product {
    public:
    void use() override {
        std::cout << "Using ConcreteProductB" << std::endl;
    }
    ~ConcreteProductB() override {
        std::cout << "Destroying ConcreteProductB" << std::endl;
    }
};

// 工厂类
class Factory {
    public:
    static Product* creatProduct(const std::string& type) {
        if (type == "A") {
            cout << "Creating ConcreteProductA" << endl;
            return new ConcreteProductA();
        } else if (type == "B") {
            cout << "Creating ConcreteProductB" << endl;
            return new ConcreteProductB();
        } else {
            return nullptr;
        }
    }
};

int main() {
    Product* productA = Factory::creatProduct("A");
    if (productA) {
        productA->use();
        delete productA; // 记得释放内存
    }

    Product* productB = Factory::creatProduct("B");
    if (productB) {
        productB->use();
        delete productB; // 记得释放内存
    }

    return 0;
}