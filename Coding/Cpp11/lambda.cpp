#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
Lambda 表达式是一种匿名函数，可以直接在需要函数的地方定义和使用。它们非常适合用于算法和回调函数。Lambda 的基本语法如下：
[capture](parameters) -> return_type {
    // function body
}
*/
int main() {
    // 基本 lambda
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "5 + 3 = " << add(5, 3) << endl;
    
    // 捕获外部变量
    int x = 10;
    // 必须捕获 x 才能在 lambda 中使用它
    auto multiply = [x](int a) {
        return a * x;
    };
    cout << "5 * 10 = " << multiply(5) << endl;
    
    // 引用捕获
    int y = 20;
    cout << "Before increment: y = " << y << endl;
    // 捕获 y 的引用，这样 lambda 内部修改 y 的值会影响外部的 y
    auto increment = [&y]() {
        y++;
    };
    increment();
    cout << "After increment: y = " << y << endl;
    
    // 捕获所有外部变量
    int a = 5, b = 10;
    // 捕获所有外部变量 by value
    auto sum = [=]() {
        return a + b;
    };
    cout << "a + b = " << sum() << endl;
    
    // 用于算法
    vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    // 使用 lambda 作为排序的比较函数，按降序排序
    sort(v.begin(), v.end(), [](int a, int b) {
        // begin() 和 end() 是 sort 函数的参数，lambda 作为第三个参数被调用来比较元素 a 和 b
        // 返回 true 表示 a 应该排在 b 前面
        return a > b;
    });
    
    cout << "Sorted vector: ";
    // 访问一个 lambda 内部的变量，必须在 lambda 定义时捕获它们
    // 下一个循环中访问 v 的元素，v 是在 lambda 定义时捕获的变量
    for (auto num : v) {
        cout << "排序完成：" << num << " " << endl;
    }
    cout << endl;
    
    return 0;
}