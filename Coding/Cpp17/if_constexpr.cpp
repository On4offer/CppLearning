#include <iostream>
#include <type_traits>
using namespace std;

/*
if constexpr是C++17引入的一个特性，它允许在编译时根据条件来选择执行的代码分支。
与传统的if语句不同，if constexpr要求条件必须是编译时常量表达式，
这样编译器可以在编译时就确定哪些代码需要被包含在最终的可执行文件中。
*/

template <typename T>   // 定义一个模板函数print，接受一个类型为T的参数t
void print(T t) {
    if constexpr (is_integral_v<T>) {   
        // is_integral_v<T>是一个类型特征，用于检查T是否是一个整数类型。
        // 如果条件为真，编译器会包含这个分支的代码，否则会忽略它。
        cout << "Integral: " << t << endl;
    } else if constexpr (is_floating_point_v<T>) {  
        // is_floating_point_v<T>是另一个类型特征，用于检查T是否是一个浮点数类型。
        // 如果条件为真，编译器会包含这个分支的代码，否则会忽略它。
        cout << "Floating point: " << t << endl;
    } else {
        cout << "Other type" << endl;
    }
}

int main() {
    print(42);      // 整数
    print(3.14);    // 浮点数
    print("Hello"); // 其他类型
    
    return 0;
}