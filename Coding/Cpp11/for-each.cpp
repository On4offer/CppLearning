#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main() {
    // 遍历 vector
    vector<int> v = {1, 2, 3, 4, 5};
    for (int num : v) {
        cout << num << ",";
    }
    cout << endl;
    
    // 遍历数组
    int arr[] = {6, 7, 8, 9, 10};
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    
    // 遍历 map
    map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    // auto& pair 是一个引用，避免了复制 map 中的元素，提高效率
    for (auto& pair : m) {
        cout << "(" << pair.first << ": " << pair.second << ")" << endl;
    }
    
    // 使用引用避免复制
    cout << "使用引用修改 vector 中的元素：" << endl;
    for (auto& num : v) {
        num *= 2;
    }
    
    cout << "Modified vector: ";
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}