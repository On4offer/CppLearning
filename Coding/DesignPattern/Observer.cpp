# include <vector>
# include <iostream>
# include <algorithm>
using namespace std;

// 观察者接口
class Observer {
public:
    // 纯虚函数，要求所有具体观察者必须实现 update 方法，
    // 以便在被观察者状态改变时接收通知并更新自身状态。
    // 该方法作用是当被观察者状态发生变化时，观察者会调用 update 方法来更新自己的状态，
    // 以便保持与被观察者的状态同步。
    virtual void update(int state) = 0;
    // 虚析构函数，确保通过基类指针删除派生类对象时，派生类的析构函数能够被正确调用，
    // 虚析构：保证多态安全释放避免资源泄漏。
    virtual ~Observer() = default;
};

// 被观察者类，维护一个观察者列表，并提供注册、注销和通知观察者的方法，
class Subject {
private:
    // 观察者列表，存储所有注册的观察者对象指针
    vector<Observer*> observers;
    // 被观察者的状态，观察者会根据这个状态来更新自己的状态
    int state;
public:
    // attach 方法用于注册观察者，将观察者对象指针添加到 observers 列表中，
    void attach(Observer* observer) {
        // push_back 方法将 observer 添加到 observers 向量的末尾，表示注册了一个新的观察者。
        observers.push_back(observer);
    }
    // detach 方法用于注销观察者，从 observers 列表中移除指定的观察者对象指针，
    void detach(Observer* observer) {
        // remove 函数将 observer 从 observers 向量中移除，返回一个新的迭代器指向新的末尾，
        // erase 方法根据 remove 返回的迭代器来删除 observers 向量中被移除的元素，完成注销操作。
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
    // notify 方法用于通知所有注册的观察者，当被观察者状态发生变化时，调用每个观察者的 update 方法，
    void notify() {
        for (Observer* observer : observers) {
            // 调用每个观察者的 update 方法，传递当前状态 state 作为参数，以便观察者能够根据新的状态来更新自己的状态。
            observer->update(state);
        }
    }
    // setState 方法用于改变被观察者的状态，并在状态改变后调用 notify 方法通知所有注册的观察者，
    void setState(int newState) {
        state = newState;
        notify();
    }
};
// 具体观察者类，继承自 Observer 接口，实现 update 方法来接收被观察者的状态更新，
class ConcreteObserver : public Observer {
private:
    // name 成员变量用于标识观察者的名称，便于在 update 方法中输出观察者的身份信息。
    string name;
    // subject 成员变量是对被观察者对象的引用，允许观察者在构造函数中注册自己，并在析构函数中注销自己，
    Subject& subject;
public:
    // 构造函数接受观察者的名称和被观察者对象的引用，并在构造函数中调用 subject.attach(this) 来注册自己为被观察者的观察者。
    ConcreteObserver(const string name, Subject& subject) : name(name), subject(subject) {
        // 在构造函数中调用 subject.attach(this) 来注册自己为被观察者的观察者。
        // 把this指针传递给 subject.attach 方法，表示当前对象（ConcreteObserver 实例）要注册为被观察者的观察者。
        subject.attach(this);
    }
    // update 方法实现了 Observer 接口中的纯虚函数，当被观察者状态发生变化时，观察者会调用 update 方法来更新自己的状态，
    void update(int state) override {
        cout << "被观察者状态发生变化，观察者 " << name << " 收到通知，新的状态为: " << state << endl;
        cout << "Observer " << name << " received update: state = " << state << endl;
    }
    ~ConcreteObserver() {
        subject.detach(this);
    }
};

int main() {
    // 创建被观察者对象 subject，它将维护一个观察者列表，并提供注册、注销和通知观察者的方法，
    Subject subject;
    // 创建两个具体观察者对象 observer1 和 observer2，并将它们注册到被观察者 subject 中
    // 观察者实例作用是当被观察者状态发生变化时，观察者会调用 update 方法来更新自己的状态，以便保持与被观察者的状态同步。
    ConcreteObserver observer1("Observer1", subject);
    ConcreteObserver observer2("Observer2", subject);

    cout << "对象创建完成，开始改变被观察者状态..." << endl;

    cout << "改变状态为 10" << endl;
    // setState 方法用于改变被观察者的状态，并在状态改变后调用 notify 方法通知所有注册的观察者，
    subject.setState(10);
    cout << "改变状态为 20" << endl;
    subject.setState(20);

    return 0;
}