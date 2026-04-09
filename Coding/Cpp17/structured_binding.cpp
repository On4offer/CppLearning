#include <iostream>
#include <tuple>
#include <map>
using namespace std;


/*
结构化绑定是C++17引入的一种语法特性，允许将一个对象的多个成员绑定到多个变量上，类似于解构赋值。
它可以用于绑定std::tuple、std::pair以及结构体等类型的对象。
结构化绑定的语法是使用auto关键字和方括号[]，在方括号中列出要绑定的变量名，等号右侧是要绑定的对象。例如：
auto [x, y] = some_tuple; // 绑定tuple的成员
*/
int main() {
    // 绑定 tuple
    // make_tuple函数可以创建一个tuple对象，包含多个不同类型的值。结构化绑定可以将tuple的成员绑定到多个变量上，方便访问和使用。
    auto t = make_tuple(1, 2.5, "Hello");
    // 定义三个变量x、y、z，分别绑定tuple t的三个成员。x绑定到第一个成员1，y绑定到第二个成员2.5，z绑定到第三个成员"Hello"。
    auto [x, y, z] = t; // 绑定tuple t的成员到变量x、y、z
    cout << "x: " << x << ", y: " << y << ", z: " << z << endl;
    
    cout << "-----------------------------" << endl;
    // 绑定 pair
    pair<int, string> p = {42, "World"};
    auto [a, b] = p;
    cout << "a: " << a << ", b: " << b << endl;
    
    cout << "-----------------------------" << endl;
    // 绑定 map 元素
    map<string, int> m = {{"a", 1}, {"b", 2}};
    for (auto [key, value] : m) {
        cout << key << ": " << value << endl;
    }
    
    return 0;
}