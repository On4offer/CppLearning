#include <iostream>
#include <vector>
using namespace std;

// 使用 typedef
// 把 vector<int> 这个类型，起一个简单的名字 IntVector
// typedef 是 C++ 中的一个关键字，用于为现有类型创建一个新的名字。它的语法如下：
// typedef existing_type new_type_name;
// 在这个例子中，existing_type 是 vector<int>，new_type_name 是 IntVector
// 这样我们就可以使用 IntVector 来代替 vector<int>，使代码更简洁和易读
typedef vector<int> IntVector;

//  函数指针别名,这两个功能完全一样，都是定义：“返回值 void，参数 int” 的函数指针类型
// 使用 using (C++11)
// using 是 C++11 引入的一个关键字，它提供了另一种方式来创建类型别名。它的语法如下：
// using new_type_name = existing_type;
// 在这个例子中，new_type_name 是 IntVector2，existing_type 是 vector<int>
// using 和 typedef 的功能相同，但 using 的语法更清晰，特别是当我们定义复杂类型别名时，using 更加直观
typedef void (*FunctionType)(int);
using FunctionType2 = void (*)(int);

// 模板别名
// template <typename T>
// typedef vector<T> VectorType; // C++11 不支持,typedef 不支持模板别名

template <typename T>
using VectorType2 = vector<T>;

void func(int x) {
    cout << "func called with " << x << endl;
}

int main() {
    // 使用 typedef
    // IntVector 是 vector<int> 的别名，我们可以直接使用 IntVector 来定义一个 vector<int> 类型的变量
    IntVector v = {1, 2, 3, 4, 5};
    cout << "使用 typedef 定义的 IntVector:" << endl;
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    cout << "使用 typedef 定义的函数指针 FunctionType:" << endl;
    // 使用函数指针类型别名
    // 定义一个函数指针 f1，让它指向函数 func,这样我们就可以通过 f1 来调用 func 函数
    FunctionType f1 = func;
    f1(10);
    
    cout << "使用 using 定义的函数指针 FunctionType2:" << endl;
    // 使用 using 定义的函数指针别名 FunctionType2 和 typedef 定义的 FunctionType 功能完全一样，我们也可以通过 f2 来调用 func 函数
    FunctionType2 f2 = func;
    f2(20);
    
    // 使用模板别名
    cout << "使用模板别名定义 VectorType2<double>:" << endl;
    VectorType2<double> v2 = {1.1, 2.2, 3.3};
    for (double num : v2) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}