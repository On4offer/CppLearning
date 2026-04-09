#include <iostream>
// #include <cstdalign> 已经被包含在 <iostream> 中了，所以不需要单独包含。
using namespace std;

// alignas(16)指定了AlignedStruct结构体的对齐要求为16字节。这意味着AlignedStruct结构体的实例将被分配在内存中地址是16的倍数的位置，以满足对齐要求。
struct alignas(16) AlignedStruct {
    char c;
    int i;
    double d;
};

int main() {
    cout << "Size of char: " << sizeof(char) << endl;
    cout << "Size of int: " << sizeof(int) << endl;
    cout << "Size of double: " << sizeof(double) << endl;
    // sizeof(AlignedStruct)返回AlignedStruct结构体的大小，alignof(AlignedStruct)返回AlignedStruct结构体的对齐要求。
    cout << "Size of AlignedStruct: " << sizeof(AlignedStruct) << endl;
    // alignof(AlignedStruct)返回AlignedStruct结构体的对齐要求。在这个示例中，由于AlignedStruct使用了alignas(16)指定了16字节的对齐要求，因此alignof(AlignedStruct)将返回16。
    cout << "Alignment of AlignedStruct: " << alignof(AlignedStruct) << endl;
    
    return 0;
}