#include <iostream>
#include <type_traits>
using namespace std;

/*
C++14 引入了变量模板（Variable Template），允许我们定义一个模板变量，这个变量可以根据不同的类型进行实例化。变量模板的语法如下：
template <typename T>
constexpr T variable_name = value;
其中，T 是一个模板参数，variable_name 是变量模板的名字，value 是一个常量表达式，用于初始化变量模板的值。变量模板可以是 constexpr，这样它就可以在编译时求值。
在这个例子中，我们定义了一个变量模板 pi，它表示圆周率的值
根据不同的类型 T，pi 的值会被实例化为不同的类型的圆周率值。我们还对 pi 进行了特化，当 T 是 int 时，pi 的值被定义为 3。
在 main 函数中，我们分别输出了 pi<double>、pi<float> 和 pi<int> 的值，展示了变量模板的灵活性和特化功能。
*/
// 变量模板，作用是定义一个模板变量 pi，根据不同的类型 T，pi 的值会被实例化为不同的类型的圆周率值
// constexpr 表示这个变量模板是一个常量表达式，可以在编译时求值
template <typename T>
constexpr T pi = T(3.14159265358979323846); // T(3.14159265358979323846) 是一个类型转换，将圆周率的值转换为类型 T

// 变量模板特化
// 当 T 是 int 时，pi 的值被定义为 3
template <>
constexpr int pi<int> = 3;

int main() {
    // 输出不同类型的 pi 值，pi<double> 是 double 类型的圆周率值，pi<float> 是 float 类型的圆周率值，pi<int> 是 int 类型的圆周率值（特化为 3）
    cout << "pi<double>: " << pi<double> << endl;
    cout << "pi<float>: " << pi<float> << endl;
    cout << "pi<int>: " << pi<int> << endl;
    
    return 0;
}