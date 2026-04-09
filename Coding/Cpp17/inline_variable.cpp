#include <iostream>
using namespace std;


/*
内联变量是C++17引入的一个特性，它允许在头文件中定义变量，并且这些变量会被自动内联。
这样可以避免在多个源文件中定义相同的变量，从而减少代码的重复和链接问题。
内联变量的语法如下：
    inline type variable_name = value;
内联变量可以是任何类型，包括基本类型、类类型、模板变量等。
*/

// 内联变量
inline int global_var = 42;

// 模板内联变量
template <typename T>
inline T pi = T(3.14159265358979323846);

int main() {
    cout << "global_var: " << global_var << endl;
    cout << "pi<double>: " << pi<double> << endl;
    cout << "pi<float>: " << pi<float> << endl;
    cout << "pi<int>: " << pi<int> << endl; // 这里会将pi<int>转换为int类型，结果为3
    // cout << "pi<T>: " << pi<T> << endl; // 这里会导致编译错误，因为T是一个未定义的模板参数，无法确定pi<T>的类型和初始值。
    return 0;
}