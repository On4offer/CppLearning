#include <iostream>
using namespace std;

/*
Fold expressions是C++17引入的一种特性，它允许将一个二元操作符应用于参数包中的所有元素。
在C++17之前，如果我们想对一个参数包中的所有元素进行操作，通常需要使用递归模板来实现
这会导致代码复杂且难以维护。而Fold expressions提供了一种更简洁和高效的方式来处理参数包。
Fold expressions的语法如下：    
    (args op ...);   // 前缀折叠
    (... op args);   // 后缀折叠
    (args1 op ... op args2); // 中缀折叠
*/
// 求和模板
template <typename... Args> // ...表示参数包，Args是一个模板参数包，可以接受任意数量和类型的模板参数。
auto sum(Args... args) {    // fold expression，使用...展开参数包，进行求和操作。
    return (args + ...);    // 这里的...表示将args中的每个参数依次相加，形成一个折叠表达式。
}

// 打印模板
template <typename... Args>
void print(Args... args) {
    (cout << ... << args) << endl;   // 折叠表达式 必须用括号包起来
    // ()表示将cout和args中的每个参数依次连接起来，形成一个折叠表达式，并在最后输出一个换行符。
}

int main() {
    cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << endl;
    print("Hello", " ", "World", "!");
    
    return 0;
}