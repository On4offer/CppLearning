# C++设计模式实战详解

本文档涵盖设计模式的核心知识点，包括创建型模式、结构型模式、行为型模式等内容，结合C++代码示例，帮助您在校招笔试面试中掌握设计模式相关知识。

## 一、设计原则

### 1. SOLID原则

- **单一职责原则 (Single Responsibility Principle)**：一个类只负责一个功能领域的相应职责
- **开放封闭原则 (Open-Closed Principle)**：软件实体应该对扩展开放，对修改封闭
- **里氏替换原则 (Liskov Substitution Principle)**：子类应该能够替换父类并出现在父类能够出现的任何地方
- **接口隔离原则 (Interface Segregation Principle)**：客户端不应该依赖它不使用的接口
- **依赖倒置原则 (Dependency Inversion Principle)**：高层模块不应该依赖低层模块，两者都应该依赖于抽象

### 2. 其他设计原则

- **组合复用原则**：优先使用组合而不是继承
- **迪米特法则**：一个对象应该对其他对象有最少的了解
- **开闭原则**：软件实体应该对扩展开放，对修改封闭

## 二、创建型模式

### 1. 单例模式

#### 1.1 懒汉式单例

```cpp
#include <iostream>
#include <mutex>
using namespace std;

class Singleton {
private:
    static Singleton* instance;
    static mutex mtx;
    
    // 私有构造函数
    Singleton() {
        cout << "Singleton created" << endl;
    }
    
    // 私有析构函数
    ~Singleton() {
        cout << "Singleton destroyed" << endl;
    }
    
    // 禁止拷贝和赋值
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);
            if (instance == nullptr) {
                instance = new Singleton();
            }
        }
        return instance;
    }
    
    static void destroyInstance() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
    
    void showMessage() {
        cout << "Hello from Singleton!" << endl;
    }
};

// 初始化静态成员
Singleton* Singleton::instance = nullptr;
mutex Singleton::mtx;

// 测试代码
int main() {
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    
    cout << "s1 address: " << s1 << endl;
    cout << "s2 address: " << s2 << endl;
    
    s1->showMessage();
    
    Singleton::destroyInstance();
    
    return 0;
}
```

#### 1.2 饿汉式单例

```cpp
#include <iostream>
using namespace std;

class Singleton {
private:
    // 静态实例，在程序启动时初始化
    static Singleton instance;
    
    // 私有构造函数
    Singleton() {
        cout << "Singleton created" << endl;
    }
    
    // 私有析构函数
    ~Singleton() {
        cout << "Singleton destroyed" << endl;
    }
    
    // 禁止拷贝和赋值
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton& getInstance() {
        return instance;
    }
    
    void showMessage() {
        cout << "Hello from Singleton!" << endl;
    }
};

// 初始化静态成员
Singleton Singleton::instance;

// 测试代码
int main() {
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();
    
    cout << "s1 address: " << &s1 << endl;
    cout << "s2 address: " << &s2 << endl;
    
    s1.showMessage();
    
    return 0;
}
```

### 2. 工厂模式

#### 2.1 简单工厂模式

```cpp
#include <iostream>
#include <string>
using namespace std;

// 产品基类
class Product {
public:
    virtual ~Product() {}
    virtual void use() = 0;
};

// 具体产品A
class ConcreteProductA : public Product {
public:
    void use() override {
        cout << "Using ConcreteProductA" << endl;
    }
};

// 具体产品B
class ConcreteProductB : public Product {
public:
    void use() override {
        cout << "Using ConcreteProductB" << endl;
    }
};

// 简单工厂
class SimpleFactory {
public:
    static Product* createProduct(const string& type) {
        if (type == "A") {
            return new ConcreteProductA();
        } else if (type == "B") {
            return new ConcreteProductB();
        }
        return nullptr;
    }
};

// 测试代码
int main() {
    Product* productA = SimpleFactory::createProduct("A");
    Product* productB = SimpleFactory::createProduct("B");
    
    productA->use();
    productB->use();
    
    delete productA;
    delete productB;
    
    return 0;
}
```

#### 2.2 工厂方法模式

```cpp
#include <iostream>
using namespace std;

// 产品基类
class Product {
public:
    virtual ~Product() {}
    virtual void use() = 0;
};

// 具体产品A
class ConcreteProductA : public Product {
public:
    void use() override {
        cout << "Using ConcreteProductA" << endl;
    }
};

// 具体产品B
class ConcreteProductB : public Product {
public:
    void use() override {
        cout << "Using ConcreteProductB" << endl;
    }
};

// 工厂基类
class Factory {
public:
    virtual ~Factory() {}
    virtual Product* createProduct() = 0;
};

// 具体工厂A
class ConcreteFactoryA : public Factory {
public:
    Product* createProduct() override {
        return new ConcreteProductA();
    }
};

// 具体工厂B
class ConcreteFactoryB : public Factory {
public:
    Product* createProduct() override {
        return new ConcreteProductB();
    }
};

// 测试代码
int main() {
    Factory* factoryA = new ConcreteFactoryA();
    Factory* factoryB = new ConcreteFactoryB();
    
    Product* productA = factoryA->createProduct();
    Product* productB = factoryB->createProduct();
    
    productA->use();
    productB->use();
    
    delete productA;
    delete productB;
    delete factoryA;
    delete factoryB;
    
    return 0;
}
```

#### 2.3 抽象工厂模式

```cpp
#include <iostream>
using namespace std;

// 抽象产品A
class AbstractProductA {
public:
    virtual ~AbstractProductA() {}
    virtual void use() = 0;
};

// 具体产品A1
class ProductA1 : public AbstractProductA {
public:
    void use() override {
        cout << "Using ProductA1" << endl;
    }
};

// 具体产品A2
class ProductA2 : public AbstractProductA {
public:
    void use() override {
        cout << "Using ProductA2" << endl;
    }
};

// 抽象产品B
class AbstractProductB {
public:
    virtual ~AbstractProductB() {}
    virtual void eat() = 0;
};

// 具体产品B1
class ProductB1 : public AbstractProductB {
public:
    void eat() override {
        cout << "Eating ProductB1" << endl;
    }
};

// 具体产品B2
class ProductB2 : public AbstractProductB {
public:
    void eat() override {
        cout << "Eating ProductB2" << endl;
    }
};

// 抽象工厂
class AbstractFactory {
public:
    virtual ~AbstractFactory() {}
    virtual AbstractProductA* createProductA() = 0;
    virtual AbstractProductB* createProductB() = 0;
};

// 具体工厂1
class ConcreteFactory1 : public AbstractFactory {
public:
    AbstractProductA* createProductA() override {
        return new ProductA1();
    }
    
    AbstractProductB* createProductB() override {
        return new ProductB1();
    }
};

// 具体工厂2
class ConcreteFactory2 : public AbstractFactory {
public:
    AbstractProductA* createProductA() override {
        return new ProductA2();
    }
    
    AbstractProductB* createProductB() override {
        return new ProductB2();
    }
};

// 测试代码
int main() {
    AbstractFactory* factory1 = new ConcreteFactory1();
    AbstractFactory* factory2 = new ConcreteFactory2();
    
    AbstractProductA* productA1 = factory1->createProductA();
    AbstractProductB* productB1 = factory1->createProductB();
    
    AbstractProductA* productA2 = factory2->createProductA();
    AbstractProductB* productB2 = factory2->createProductB();
    
    productA1->use();
    productB1->eat();
    productA2->use();
    productB2->eat();
    
    delete productA1;
    delete productB1;
    delete productA2;
    delete productB2;
    delete factory1;
    delete factory2;
    
    return 0;
}
```

### 3. 建造者模式

```cpp
#include <iostream>
#include <string>
using namespace std;

// 产品
class Product {
private:
    string partA;
    string partB;
    string partC;

public:
    void setPartA(const string& a) {
        partA = a;
    }
    
    void setPartB(const string& b) {
        partB = b;
    }
    
    void setPartC(const string& c) {
        partC = c;
    }
    
    void show() {
        cout << "Product parts: " << partA << ", " << partB << ", " << partC << endl;
    }
};

// 建造者接口
class Builder {
public:
    virtual ~Builder() {}
    virtual void buildPartA() = 0;
    virtual void buildPartB() = 0;
    virtual void buildPartC() = 0;
    virtual Product* getResult() = 0;
};

// 具体建造者1
class ConcreteBuilder1 : public Builder {
private:
    Product* product;

public:
    ConcreteBuilder1() {
        product = new Product();
    }
    
    ~ConcreteBuilder1() {
        delete product;
    }
    
    void buildPartA() override {
        product->setPartA("PartA1");
    }
    
    void buildPartB() override {
        product->setPartB("PartB1");
    }
    
    void buildPartC() override {
        product->setPartC("PartC1");
    }
    
    Product* getResult() override {
        return product;
    }
};

// 具体建造者2
class ConcreteBuilder2 : public Builder {
private:
    Product* product;

public:
    ConcreteBuilder2() {
        product = new Product();
    }
    
    ~ConcreteBuilder2() {
        delete product;
    }
    
    void buildPartA() override {
        product->setPartA("PartA2");
    }
    
    void buildPartB() override {
        product->setPartB("PartB2");
    }
    
    void buildPartC() override {
        product->setPartC("PartC2");
    }
    
    Product* getResult() override {
        return product;
    }
};

// 指挥者
class Director {
private:
    Builder* builder;

public:
    void setBuilder(Builder* b) {
        builder = b;
    }
    
    void construct() {
        builder->buildPartA();
        builder->buildPartB();
        builder->buildPartC();
    }
};

// 测试代码
int main() {
    Director director;
    
    // 使用建造者1
    ConcreteBuilder1 builder1;
    director.setBuilder(&builder1);
    director.construct();
    Product* product1 = builder1.getResult();
    product1->show();
    
    // 使用建造者2
    ConcreteBuilder2 builder2;
    director.setBuilder(&builder2);
    director.construct();
    Product* product2 = builder2.getResult();
    product2->show();
    
    return 0;
}
```

### 4. 原型模式

```cpp
#include <iostream>
#include <string>
using namespace std;

// 原型接口
class Prototype {
public:
    virtual ~Prototype() {}
    virtual Prototype* clone() = 0;
    virtual void show() = 0;
};

// 具体原型1
class ConcretePrototype1 : public Prototype {
private:
    string name;

public:
    ConcretePrototype1(const string& n) : name(n) {}
    
    Prototype* clone() override {
        return new ConcretePrototype1(*this);
    }
    
    void show() override {
        cout << "ConcretePrototype1: " << name << endl;
    }
    
    void setName(const string& n) {
        name = n;
    }
};

// 具体原型2
class ConcretePrototype2 : public Prototype {
private:
    int value;

public:
    ConcretePrototype2(int v) : value(v) {}
    
    Prototype* clone() override {
        return new ConcretePrototype2(*this);
    }
    
    void show() override {
        cout << "ConcretePrototype2: " << value << endl;
    }
    
    void setValue(int v) {
        value = v;
    }
};

// 测试代码
int main() {
    // 创建原型
    ConcretePrototype1* p1 = new ConcretePrototype1("Prototype1");
    ConcretePrototype2* p2 = new ConcretePrototype2(100);
    
    // 克隆
    ConcretePrototype1* p1Clone = dynamic_cast<ConcretePrototype1*>(p1->clone());
    ConcretePrototype2* p2Clone = dynamic_cast<ConcretePrototype2*>(p2->clone());
    
    // 修改克隆对象
    p1Clone->setName("Clone1");
    p2Clone->setValue(200);
    
    // 显示
    cout << "Original: " << endl;
    p1->show();
    p2->show();
    
    cout << "Cloned: " << endl;
    p1Clone->show();
    p2Clone->show();
    
    // 释放内存
    delete p1;
    delete p2;
    delete p1Clone;
    delete p2Clone;
    
    return 0;
}
```

## 三、结构型模式

### 1. 适配器模式

```cpp
#include <iostream>
using namespace std;

// 目标接口
class Target {
public:
    virtual ~Target() {}
    virtual void request() = 0;
};

// 具体目标
class ConcreteTarget : public Target {
public:
    void request() override {
        cout << "ConcreteTarget::request()" << endl;
    }
};

// 适配者
class Adaptee {
public:
    void specificRequest() {
        cout << "Adaptee::specificRequest()" << endl;
    }
};

// 类适配器
class ClassAdapter : public Target, private Adaptee {
public:
    void request() override {
        specificRequest();
    }
};

// 对象适配器
class ObjectAdapter : public Target {
private:
    Adaptee* adaptee;

public:
    ObjectAdapter(Adaptee* a) : adaptee(a) {}
    
    void request() override {
        adaptee->specificRequest();
    }
};

// 测试代码
int main() {
    // 使用类适配器
    cout << "Using ClassAdapter: " << endl;
    ClassAdapter classAdapter;
    classAdapter.request();
    
    // 使用对象适配器
    cout << "Using ObjectAdapter: " << endl;
    Adaptee adaptee;
    ObjectAdapter objectAdapter(&adaptee);
    objectAdapter.request();
    
    return 0;
}
```

### 2. 装饰器模式

```cpp
#include <iostream>
#include <string>
using namespace std;

// 组件接口
class Component {
public:
    virtual ~Component() {}
    virtual string operation() = 0;
};

// 具体组件
class ConcreteComponent : public Component {
public:
    string operation() override {
        return "ConcreteComponent";
    }
};

// 装饰器
class Decorator : public Component {
protected:
    Component* component;

public:
    Decorator(Component* c) : component(c) {}
    
    string operation() override {
        return component->operation();
    }
};

// 具体装饰器A
class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(Component* c) : Decorator(c) {}
    
    string operation() override {
        return "ConcreteDecoratorA(" + Decorator::operation() + ")";
    }
};

// 具体装饰器B
class ConcreteDecoratorB : public Decorator {
public:
    ConcreteDecoratorB(Component* c) : Decorator(c) {}
    
    string operation() override {
        return "ConcreteDecoratorB(" + Decorator::operation() + ")";
    }
};

// 测试代码
int main() {
    // 创建具体组件
    Component* component = new ConcreteComponent();
    cout << "Original: " << component->operation() << endl;
    
    // 用装饰器A装饰
    Component* decoratorA = new ConcreteDecoratorA(component);
    cout << "Decorated with A: " << decoratorA->operation() << endl;
    
    // 用装饰器B装饰
    Component* decoratorB = new ConcreteDecoratorB(decoratorA);
    cout << "Decorated with A and B: " << decoratorB->operation() << endl;
    
    // 释放内存
    delete decoratorB;
    delete decoratorA;
    delete component;
    
    return 0;
}
```

### 3. 代理模式

```cpp
#include <iostream>
using namespace std;

// 主题接口
class Subject {
public:
    virtual ~Subject() {}
    virtual void request() = 0;
};

// 真实主题
class RealSubject : public Subject {
public:
    void request() override {
        cout << "RealSubject::request()" << endl;
    }
};

// 代理
class Proxy : public Subject {
private:
    RealSubject* realSubject;
    
    // 检查访问权限
    bool checkAccess() {
        cout << "Proxy::checkAccess()" << endl;
        return true;
    }
    
    // 记录访问日志
    void logAccess() {
        cout << "Proxy::logAccess()" << endl;
    }

public:
    Proxy() : realSubject(nullptr) {}
    
    ~Proxy() {
        delete realSubject;
    }
    
    void request() override {
        if (checkAccess()) {
            if (!realSubject) {
                realSubject = new RealSubject();
            }
            realSubject->request();
            logAccess();
        }
    }
};

// 测试代码
int main() {
    cout << "Using Proxy: " << endl;
    Proxy proxy;
    proxy.request();
    
    return 0;
}
```

### 4. 组合模式

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 组件
class Component {
protected:
    string name;

public:
    Component(const string& n) : name(n) {}
    virtual ~Component() {}
    
    virtual void add(Component* component) {}
    virtual void remove(Component* component) {}
    virtual void display(int depth) = 0;
};

// 叶子节点
class Leaf : public Component {
public:
    Leaf(const string& n) : Component(n) {}
    
    void display(int depth) override {
        for (int i = 0; i < depth; i++) {
            cout << "-";
        }
        cout << name << endl;
    }
};

// 复合节点
class Composite : public Component {
private:
    vector<Component*> children;

public:
    Composite(const string& n) : Component(n) {}
    
    ~Composite() {
        for (Component* child : children) {
            delete child;
        }
    }
    
    void add(Component* component) override {
        children.push_back(component);
    }
    
    void remove(Component* component) override {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (*it == component) {
                children.erase(it);
                delete component;
                break;
            }
        }
    }
    
    void display(int depth) override {
        for (int i = 0; i < depth; i++) {
            cout << "-";
        }
        cout << name << endl;
        
        for (Component* child : children) {
            child->display(depth + 2);
        }
    }
};

// 测试代码
int main() {
    // 创建根节点
    Composite* root = new Composite("Root");
    
    // 添加叶子节点
    root->add(new Leaf("Leaf A"));
    root->add(new Leaf("Leaf B"));
    
    // 创建子节点
    Composite* branch1 = new Composite("Branch 1");
    branch1->add(new Leaf("Leaf C"));
    branch1->add(new Leaf("Leaf D"));
    
    // 添加子节点到根
    root->add(branch1);
    
    // 创建另一个子节点
    Composite* branch2 = new Composite("Branch 2");
    branch2->add(new Leaf("Leaf E"));
    
    // 添加子节点到根
    root->add(branch2);
    
    // 显示结构
    cout << "Composite structure: " << endl;
    root->display(0);
    
    // 删除一个节点
    cout << "\nAfter removing Branch 2: " << endl;
    root->remove(branch2);
    root->display(0);
    
    // 释放内存
    delete root;
    
    return 0;
}
```

### 5. 外观模式

```cpp
#include <iostream>
using namespace std;

// 子系统1
class SubSystem1 {
public:
    void operation1() {
        cout << "SubSystem1::operation1()" << endl;
    }
};

// 子系统2
class SubSystem2 {
public:
    void operation2() {
        cout << "SubSystem2::operation2()" << endl;
    }
};

// 子系统3
class SubSystem3 {
public:
    void operation3() {
        cout << "SubSystem3::operation3()" << endl;
    }
};

// 外观
class Facade {
private:
    SubSystem1* subSystem1;
    SubSystem2* subSystem2;
    SubSystem3* subSystem3;

public:
    Facade() {
        subSystem1 = new SubSystem1();
        subSystem2 = new SubSystem2();
        subSystem3 = new SubSystem3();
    }
    
    ~Facade() {
        delete subSystem1;
        delete subSystem2;
        delete subSystem3;
    }
    
    void operationA() {
        cout << "Facade::operationA()" << endl;
        subSystem1->operation1();
        subSystem2->operation2();
    }
    
    void operationB() {
        cout << "Facade::operationB()" << endl;
        subSystem2->operation2();
        subSystem3->operation3();
    }
};

// 测试代码
int main() {
    Facade facade;
    
    cout << "Operation A: " << endl;
    facade.operationA();
    
    cout << "\nOperation B: " << endl;
    facade.operationB();
    
    return 0;
}
```

## 四、行为型模式

### 1. 观察者模式

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 观察者接口
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(const string& message) = 0;
};

// 主题接口
class Subject {
public:
    virtual ~Subject() {}
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
};

// 具体主题
class ConcreteSubject : public Subject {
private:
    vector<Observer*> observers;
    string message;

public:
    void attach(Observer* observer) override {
        observers.push_back(observer);
    }
    
    void detach(Observer* observer) override {
        for (auto it = observers.begin(); it != observers.end(); ++it) {
            if (*it == observer) {
                observers.erase(it);
                break;
            }
        }
    }
    
    void notify() override {
        for (Observer* observer : observers) {
            observer->update(message);
        }
    }
    
    void setMessage(const string& msg) {
        message = msg;
        notify();
    }
};

// 具体观察者1
class ConcreteObserver1 : public Observer {
private:
    string name;

public:
    ConcreteObserver1(const string& n) : name(n) {}
    
    void update(const string& message) override {
        cout << name << " received: " << message << endl;
    }
};

// 具体观察者2
class ConcreteObserver2 : public Observer {
private:
    string name;

public:
    ConcreteObserver2(const string& n) : name(n) {}
    
    void update(const string& message) override {
        cout << name << " received: " << message << endl;
    }
};

// 测试代码
int main() {
    // 创建主题
    ConcreteSubject subject;
    
    // 创建观察者
    ConcreteObserver1 observer1("Observer 1");
    ConcreteObserver2 observer2("Observer 2");
    
    // 注册观察者
    subject.attach(&observer1);
    subject.attach(&observer2);
    
    // 发送消息
    subject.setMessage("Hello World!");
    
    // 移除观察者1
    subject.detach(&observer1);
    
    // 发送消息
    subject.setMessage("Hello Again!");
    
    return 0;
}
```

### 2. 策略模式

```cpp
#include <iostream>
using namespace std;

// 策略接口
class Strategy {
public:
    virtual ~Strategy() {}
    virtual int doOperation(int num1, int num2) = 0;
};

// 具体策略1：加法
class OperationAdd : public Strategy {
public:
    int doOperation(int num1, int num2) override {
        return num1 + num2;
    }
};

// 具体策略2：减法
class OperationSubtract : public Strategy {
public:
    int doOperation(int num1, int num2) override {
        return num1 - num2;
    }
};

// 具体策略3：乘法
class OperationMultiply : public Strategy {
public:
    int doOperation(int num1, int num2) override {
        return num1 * num2;
    }
};

// 上下文
class Context {
private:
    Strategy* strategy;

public:
    Context(Strategy* s) : strategy(s) {}
    
    void setStrategy(Strategy* s) {
        strategy = s;
    }
    
    int executeStrategy(int num1, int num2) {
        return strategy->doOperation(num1, num2);
    }
};

// 测试代码
int main() {
    // 创建上下文
    Context context(new OperationAdd());
    cout << "10 + 5 = " << context.executeStrategy(10, 5) << endl;
    
    // 切换策略为减法
    context.setStrategy(new OperationSubtract());
    cout << "10 - 5 = " << context.executeStrategy(10, 5) << endl;
    
    // 切换策略为乘法
    context.setStrategy(new OperationMultiply());
    cout << "10 * 5 = " << context.executeStrategy(10, 5) << endl;
    
    return 0;
}
```

### 3. 命令模式

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 接收者
class Receiver {
public:
    void action() {
        cout << "Receiver::action()" << endl;
    }
};

// 命令接口
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

// 具体命令
class ConcreteCommand : public Command {
private:
    Receiver* receiver;

public:
    ConcreteCommand(Receiver* r) : receiver(r) {}
    
    void execute() override {
        receiver->action();
    }
};

// 调用者
class Invoker {
private:
    vector<Command*> commands;

public:
    void addCommand(Command* command) {
        commands.push_back(command);
    }
    
    void executeCommands() {
        for (Command* command : commands) {
            command->execute();
        }
    }
};

// 测试代码
int main() {
    // 创建接收者
    Receiver receiver;
    
    // 创建命令
    ConcreteCommand command(&receiver);
    
    // 创建调用者
    Invoker invoker;
    invoker.addCommand(&command);
    
    // 执行命令
    invoker.executeCommands();
    
    return 0;
}
```

### 4. 模板方法模式

```cpp
#include <iostream>
using namespace std;

// 抽象类
class AbstractClass {
public:
    virtual ~AbstractClass() {}
    
    // 模板方法
    void templateMethod() {
        primitiveOperation1();
        primitiveOperation2();
        primitiveOperation3();
    }
    
protected:
    virtual void primitiveOperation1() = 0;
    virtual void primitiveOperation2() = 0;
    
    // 钩子方法
    virtual void primitiveOperation3() {
        cout << "AbstractClass::primitiveOperation3()" << endl;
    }
};

// 具体类1
class ConcreteClass1 : public AbstractClass {
protected:
    void primitiveOperation1() override {
        cout << "ConcreteClass1::primitiveOperation1()" << endl;
    }
    
    void primitiveOperation2() override {
        cout << "ConcreteClass1::primitiveOperation2()" << endl;
    }
};

// 具体类2
class ConcreteClass2 : public AbstractClass {
protected:
    void primitiveOperation1() override {
        cout << "ConcreteClass2::primitiveOperation1()" << endl;
    }
    
    void primitiveOperation2() override {
        cout << "ConcreteClass2::primitiveOperation2()" << endl;
    }
    
    void primitiveOperation3() override {
        cout << "ConcreteClass2::primitiveOperation3()" << endl;
    }
};

// 测试代码
int main() {
    // 使用具体类1
    AbstractClass* concrete1 = new ConcreteClass1();
    cout << "ConcreteClass1: " << endl;
    concrete1->templateMethod();
    
    // 使用具体类2
    AbstractClass* concrete2 = new ConcreteClass2();
    cout << "\nConcreteClass2: " << endl;
    concrete2->templateMethod();
    
    // 释放内存
    delete concrete1;
    delete concrete2;
    
    return 0;
}
```

### 5. 迭代器模式

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 迭代器接口
class Iterator {
public:
    virtual ~Iterator() {}
    virtual bool hasNext() = 0;
    virtual int next() = 0;
};

// 聚合接口
class Aggregate {
public:
    virtual ~Aggregate() {}
    virtual Iterator* createIterator() = 0;
};

// 具体聚合
class ConcreteAggregate : public Aggregate {
private:
    vector<int> items;

public:
    void addItem(int item) {
        items.push_back(item);
    }
    
    int getItem(int index) {
        return items[index];
    }
    
    int getSize() {
        return items.size();
    }
    
    Iterator* createIterator() override;
};

// 具体迭代器
class ConcreteIterator : public Iterator {
private:
    ConcreteAggregate* aggregate;
    int index;

public:
    ConcreteIterator(ConcreteAggregate* agg) : aggregate(agg), index(0) {}
    
    bool hasNext() override {
        return index < aggregate->getSize();
    }
    
    int next() override {
        if (hasNext()) {
            return aggregate->getItem(index++);
        }
        return -1;
    }
};

// 实现createIterator方法
Iterator* ConcreteAggregate::createIterator() {
    return new ConcreteIterator(this);
}

// 测试代码
int main() {
    // 创建聚合
    ConcreteAggregate aggregate;
    aggregate.addItem(1);
    aggregate.addItem(2);
    aggregate.addItem(3);
    aggregate.addItem(4);
    aggregate.addItem(5);
    
    // 创建迭代器
    Iterator* iterator = aggregate.createIterator();
    
    // 遍历
    cout << "Items: ";
    while (iterator->hasNext()) {
        cout << iterator->next() << " ";
    }
    cout << endl;
    
    // 释放内存
    delete iterator;
    
    return 0;
}
```

## 五、设计模式应用

### 1. 单例模式的应用

**应用场景**：配置管理、日志管理、数据库连接池等。

**示例**：日志管理器

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
using namespace std;

class Logger {
private:
    static Logger* instance;
    static mutex mtx;
    ofstream logFile;
    
    // 私有构造函数
    Logger() {
        logFile.open("log.txt", ios::app);
    }
    
    // 私有析构函数
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    // 禁止拷贝和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    static Logger* getInstance() {
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);
            if (instance == nullptr) {
                instance = new Logger();
            }
        }
        return instance;
    }
    
    static void destroyInstance() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
    
    void log(const string& message) {
        logFile << message << endl;
        cout << message << endl;
    }
};

// 初始化静态成员
Logger* Logger::instance = nullptr;
mutex Logger::mtx;

// 测试代码
int main() {
    Logger* logger = Logger::getInstance();
    logger->log("Application started");
    logger->log("Processing data");
    logger->log("Application ended");
    
    Logger::destroyInstance();
    
    return 0;
}
```

### 2. 工厂模式的应用

**应用场景**：创建复杂对象、依赖注入等。

**示例**：图形工厂

```cpp
#include <iostream>
using namespace std;

// 图形接口
class Shape {
public:
    virtual ~Shape() {}
    virtual void draw() = 0;
};

// 具体图形：圆形
class Circle : public Shape {
public:
    void draw() override {
        cout << "Drawing Circle" << endl;
    }
};

// 具体图形：矩形
class Rectangle : public Shape {
public:
    void draw() override {
        cout << "Drawing Rectangle" << endl;
    }
};

// 具体图形：三角形
class Triangle : public Shape {
public:
    void draw() override {
        cout << "Drawing Triangle" << endl;
    }
};

// 图形工厂
class ShapeFactory {
public:
    static Shape* createShape(const string& type) {
        if (type == "circle") {
            return new Circle();
        } else if (type == "rectangle") {
            return new Rectangle();
        } else if (type == "triangle") {
            return new Triangle();
        }
        return nullptr;
    }
};

// 测试代码
int main() {
    Shape* circle = ShapeFactory::createShape("circle");
    Shape* rectangle = ShapeFactory::createShape("rectangle");
    Shape* triangle = ShapeFactory::createShape("triangle");
    
    circle->draw();
    rectangle->draw();
    triangle->draw();
    
    delete circle;
    delete rectangle;
    delete triangle;
    
    return 0;
}
```

### 3. 观察者模式的应用

**应用场景**：事件处理、消息通知等。

**示例**：天气监测系统

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 观察者接口
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(float temperature, float humidity, float pressure) = 0;
};

// 主题接口
class Subject {
public:
    virtual ~Subject() {}
    virtual void registerObserver(Observer* observer) = 0;
    virtual void removeObserver(Observer* observer) = 0;
    virtual void notifyObservers() = 0;
};

// 具体主题：天气数据
class WeatherData : public Subject {
private:
    vector<Observer*> observers;
    float temperature;
    float humidity;
    float pressure;

public:
    void registerObserver(Observer* observer) override {
        observers.push_back(observer);
    }
    
    void removeObserver(Observer* observer) override {
        for (auto it = observers.begin(); it != observers.end(); ++it) {
            if (*it == observer) {
                observers.erase(it);
                break;
            }
        }
    }
    
    void notifyObservers() override {
        for (Observer* observer : observers) {
            observer->update(temperature, humidity, pressure);
        }
    }
    
    void setMeasurements(float temp, float hum, float pres) {
        temperature = temp;
        humidity = hum;
        pressure = pres;
        measurementsChanged();
    }
    
    void measurementsChanged() {
        notifyObservers();
    }
};

// 具体观察者：当前天气显示
class CurrentConditionsDisplay : public Observer {
private:
    float temperature;
    float humidity;

public:
    void update(float temp, float hum, float pres) override {
        temperature = temp;
        humidity = hum;
        display();
    }
    
    void display() {
        cout << "Current conditions: " << temperature << "F degrees and " << humidity << "% humidity" << endl;
    }
};

// 具体观察者：统计显示
class StatisticsDisplay : public Observer {
private:
    float maxTemp;
    float minTemp;
    float tempSum;
    int numReadings;

public:
    StatisticsDisplay() : maxTemp(-1000), minTemp(1000), tempSum(0), numReadings(0) {}
    
    void update(float temp, float hum, float pres) override {
        tempSum += temp;
        numReadings++;
        
        if (temp > maxTemp) {
            maxTemp = temp;
        }
        if (temp < minTemp) {
            minTemp = temp;
        }
        
        display();
    }
    
    void display() {
        cout << "Statistics: Avg = " << (tempSum / numReadings) << "F, Max = " << maxTemp << "F, Min = " << minTemp << "F" << endl;
    }
};

// 测试代码
int main() {
    // 创建天气数据
    WeatherData weatherData;
    
    // 创建显示
    CurrentConditionsDisplay currentDisplay;
    StatisticsDisplay statsDisplay;
    
    // 注册观察者
    weatherData.registerObserver(&currentDisplay);
    weatherData.registerObserver(&statsDisplay);
    
    // 设置测量值
    weatherData.setMeasurements(80, 65, 30.4);
    weatherData.setMeasurements(82, 70, 29.2);
    weatherData.setMeasurements(78, 90, 29.2);
    
    return 0;
}
```

## 六、常见考点

### 1. 单例模式的线程安全实现

**答案**：使用双重检查锁定（Double-Checked Locking）或饿汉式单例。

### 2. 工厂模式与抽象工厂模式的区别

**答案**：
- 工厂方法模式：一个抽象产品类，可以派生出多个具体产品类；一个抽象工厂类，可以派生出多个具体工厂类。每个具体工厂类只能创建一个具体产品类的实例。
- 抽象工厂模式：多个抽象产品类，每个抽象产品类可以派生出多个具体产品类；一个抽象工厂类，可以派生出多个具体工厂类。每个具体工厂类可以创建多个具体产品类的实例。

### 3. 装饰器模式与适配器模式的区别

**答案**：
- 装饰器模式：动态地给对象添加额外的职责，不改变接口。
- 适配器模式：将一个类的接口转换成客户端期望的另一个接口，改变接口。

### 4. 观察者模式的应用场景

**答案**：事件处理系统、消息通知系统、GUI组件交互、订阅-发布系统等。

### 5. 策略模式的优势

**答案**：
- 算法可以独立于使用它的客户端而变化
- 可以动态切换算法
- 避免使用多重条件判断
- 符合开闭原则

## 七、总结

设计模式是软件设计中的最佳实践，掌握设计模式对于编写高质量、可维护的代码至关重要。通过本文档的学习，您应该能够：

1. **理解设计原则**：掌握SOLID原则和其他设计原则
2. **掌握创建型模式**：单例模式、工厂模式、建造者模式、原型模式
3. **掌握结构型模式**：适配器模式、装饰器模式、代理模式、组合模式、外观模式
4. **掌握行为型模式**：观察者模式、策略模式、命令模式、模板方法模式、迭代器模式
5. **应用设计模式**：在实际项目中合理使用设计模式
6. **应对常见考点**：熟悉设计模式面试中的高频问题

在准备校招笔试面试时，建议结合实际项目经验，加深对设计模式的理解。通过系统的学习和实践，您一定能够在设计模式相关的面试中取得优异的成绩。