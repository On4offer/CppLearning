#include <iostream>
#include <vector>
#include <ranges>
// 作用：提供范围（ranges）和视图（views）的功能，支持链式操作、过滤、转换等，类似于函数式编程的流式处理
// 特点：零拷贝、惰性求值、不修改原数据
using namespace std;

int main() {
    // C++20引入了ranges库，提供了一种更简洁和高效的方式来处理范围（ranges）和视图（views）。
    // ranges库允许我们对容器进行链式操作，类似于函数式编程中的流式操作。
    // 我们可以使用ranges库来过滤、转换和组合容器中的元素，而不需要创建临时容器。
    // 语法：容器 | views::操作()
    
    // C++20 views（视图）= 零拷贝 + 不创建副本 + 不修改原数据
    // result 里面没有存 4 8 12 16 20。它只存了一句话：
    // “等别人遍历我的时候，我再去 v 里面拿偶数，然后乘 2 返回”

    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 过滤偶数并乘以2
    // filter是一个视图适配器，用于过滤满足条件的元素；
    // transform是另一个视图适配器，用于对元素进行转换。操作的是过滤后的结果，而不是原始容器。
    auto result = v | views::filter([](int x) { return x % 2 == 0; })
                    | views::transform([](int x) { return x * 2; });
    
    cout << "Filtered and transformed: ";
    for (auto num : result) {
        cout << num << " ";
    }
    cout << endl;
    // ranges是一个范围库，提供了许多算法和适配器来处理范围（ranges）。distance是ranges库中的一个算法，用于计算范围内元素的数量。
    cout << "result 大小1：" << ranges::distance(result) << endl;
    cout << "result 大小2：" << distance(result.begin(), result.end()) << endl;
    // cout << "result.size() = " << result.size() << endl; // 结果是一个视图，没有size()成员函数，编译错误   
    // cout << "result 大小：" << ranges::size(result) << endl; // 结果是一个视图，没有size()成员函数，编译错误
    cout << "---------" << endl;
    
    // 取前5个元素
    auto first5 = v | views::take(5);
    cout << "First 5 elements: ";
    for (auto num : first5) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}