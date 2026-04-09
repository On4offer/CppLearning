#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class MyString {
private:
    char* data;
    int length;

public:
    // 构造函数
    MyString(const char* str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "Constructor called。正在初始化" << endl;
    }
    
    // 析构函数
    ~MyString() {
        if (data) {
            delete[] data;
            cout << "Destructor called。正在销毁" << endl;
        }
    }
    
    // 复制构造函数，用来复制一个已有的对象，创建新对象
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        cout << "Copy constructor called。正在复制" << endl;
    }
    
    // 移动构造函数
    MyString(MyString&& other) noexcept {
        length = other.length;
        data = other.data;
        other.data = nullptr;
        other.length = 0;
        cout << "Move constructor called。正在移动" << endl;
    }
    
    // 复制赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            cout << "Copy assignment called。正在复制" << endl;
        }
        return *this;
    }
    
    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = other.data;
            other.data = nullptr;
            other.length = 0;
            cout << "Move assignment called" << endl;
        }
        return *this;
    }
    
    const char* c_str() const {
        return data;
    }
};

MyString createString() {
    cout << "Creating a temporary MyString object in createString()" << endl;
    return MyString("Hello World");
}

int main() {
    // 复制构造
    cout << "Testing copy constructor:" << endl;
    MyString s1("Hello");
    cout << "s1的C字符串: " << s1.c_str() << endl;
    MyString s2 = s1; // 复制构造
    cout << "s2的C字符串: " << s2.c_str() << "  s1的C字符串: " << s1.c_str() << endl;
    
    // 移动构造
    cout << "Testing move constructor:" << endl;
    MyString s3 = createString(); // 移动构造
    cout << "s3的C字符串: " << s3.c_str() << endl;

    // 移动赋值
    cout << "Testing move assignment:" << endl;
    MyString s4("Test");
    s4 = move(s1); // 移动赋值
    cout << "s4的C字符串: " << s4.c_str() << "  s1的C字符串是否为空: " << (s1.c_str() == nullptr ? "是" : "否") << endl;
    

    cout << "Final state of strings:" << endl;
    cout << "s2的C字符串: " << s2.c_str() << endl;
    cout << "s3的C字符串: " << s3.c_str() << endl;
    cout << "s4的C字符串: " << s4.c_str() << endl;
    
    cout << "End of main function。准备退出并销毁对象" << endl;
    return 0;
    cout << "This line will never be executed" << endl; // 这行代码永远不会被执行
}