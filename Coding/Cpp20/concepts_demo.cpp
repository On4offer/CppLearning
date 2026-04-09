#include <iostream>
#include <concepts>
using namespace std;

// 定义概念
template <typename T>
concept Integral = is_integral_v<T>;
// Integral概念要求类型T必须是一个整数类型

template <typename T>
concept SignedIntegral = Integral<T> && is_signed_v<T>;
// SignedIntegral概念要求类型T必须满足Integral概念，并且是一个有符号整数类型

// 使用概念
template <Integral T>   // 约束模板参数T必须满足Integral概念
T add(T a, T b) {
    return a + b;
}

// 约束自动类型推导
// SignedIntegral auto a是一个有符号整数类型的自动类型推导参数
auto multiply(SignedIntegral auto a, SignedIntegral auto b) {
    return a * b;
}

int main() {
    cout << "add(5, 3) = " << add(5, 3) << endl;
    // cout << add(3.14, 2.71) << endl; // 编译错误，不是整数
    
    cout << "multiply(5, -3) = " << multiply(5, -3) << endl;
    // cout << multiply(5U, 3) << endl; // 编译错误，不是有符号整数
    
    return 0;
}