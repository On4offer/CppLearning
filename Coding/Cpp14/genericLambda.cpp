#include <iostream>
using namespace std;

/*
C++14 引入了泛型 lambda（Generic Lambda），允许我们在 lambda 表达式中使用 auto 作为参数类型
这样 lambda 就可以接受不同类型的参数，而不需要为每种类型定义一个单独的 lambda。这使得 lambda 更加灵活和通用。
在 C++14 之前，lambda 的参数类型必须明确指定，如果我们想要一个 lambda 能处理多种类型
我们需要定义多个 lambda，或者使用模板函数来实现。而泛型 lambda 通过使用 auto 参数类型，允许我们在同一个 lambda 中处理不同类型的参数。
泛型 lambda 的语法如下：
[capture](auto parameters) -> return_type {
    // function body
}
其中，auto parameters 表示 lambda 的参数类型可以是任意类型，编译器会根据调用 lambda 时传入的参数类型自动推断出参数类型。
在这个例子中，我们定义了一个泛型 lambda add，它接受两个参数 a 和 b，参数类型都是 auto
这意味着 add 可以接受任何类型的参数，只要它们支持 + 运算符即可。
我们在 main 函数中调用 add 来计算整数、浮点数和字符串的和，展示了泛型 lambda 的灵活性和通用性。
*/
int main() {
    // 泛型 lambda
    auto add = [](auto a, auto b) {
        return a + b;
    };
    
    cout << "5 + 3 = " << add(5, 3) << endl;
    cout << "3.14 + 2.71 = " << add(3.14, 2.71) << endl;
    cout << "Hello " + string("World") << endl;
    
    return 0;
}