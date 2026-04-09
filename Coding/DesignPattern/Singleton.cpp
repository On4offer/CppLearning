#include <iostream>
using namespace std;
class Singleton
{
private:
    static Singleton* instance;
    Singleton(){
        cout << "Singleton Constructor" << endl;
    }
    // 禁止拷贝构造函数和赋值运算符
    // 这两行代码将拷贝构造函数和赋值运算符删除，防止外部代码复制 Singleton 实例
    // 通过删除拷贝构造函数和赋值运算符，Singleton 类无法被复制或赋值，从而确保了只有一个实例存在。
    // 这是一种常见的实现单例模式的方法，确保了 Singleton 类只能有一个实例，并且提供了全局访问点。
    // 删除的是编译器自动生成的默认拷贝构造函数和赋值运算符，
    // 禁止了对象的复制和赋值操作，从而保证了单例模式的正确性。
    // 外部可能会尝试复制 Singleton 实例，如果没有删除拷贝构造函数和赋值运算符，
    // 编译器会生成默认的拷贝构造函数和赋值运算符，这可能导致多个实例的存在，
    // 违背了单例模式的原则。
    // =delete 的函数，不管写 public /private/protected，效果完全一样！
    // 因为 =delete 的意思是：这个函数被彻底删掉了，谁都不能调用，包括内部、外部、子类
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
public:
    static Singleton* getInstance(){
        if(instance == nullptr){
            instance = new Singleton();
        }
        return instance;
    }
    void someMethod(){
        cout << "Some method in Singleton" << endl;
    }

    ~Singleton(){
        cout << "Singleton Destructor" << endl;
    }
};

Singleton* Singleton::instance = nullptr;

int main(){
    Singleton* s1 = Singleton::getInstance();
    s1->someMethod();

    Singleton* s2 = Singleton::getInstance();
    s2->someMethod();

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s1 and s2 are the same instance: " << (s1 == s2 ? "Yes" : "No") << endl;

    return 0;
}
