#include <iostream>
#include <vector>
using namespace std;

// 自动推导返回类型
auto add(int a, int b) {
    return a + b;
}

// 复杂返回类型推导
auto getVector() {
    return vector<int>{1, 2, 3, 4, 5};
}

int main() {
    cout << "add(5, 3) = " << add(5, 3) << "类型为: " << typeid(add(5, 3)).name() << endl;
    
    auto v = getVector();
    for (int num : v) {
        cout << num << "类型为: " << typeid(num).name() << endl;
    }
    cout << endl;
    
    return 0;
}