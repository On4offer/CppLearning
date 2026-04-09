#include <iostream>
#include <vector>
#include <cstddef>
using namespace std;

// 这是一个固定大小内存池的示例程序。内存池在内存分配时更高效，适合反复分配相同大小对象的场景。
// 思路：先一次性申请一大块内存，然后把这大块切成很多固定大小的小块，后续每次分配从这块内存中拿一个小块。
class MemoryPool {
private:
    // blocks 用来保存每次从系统申请的大块内存的指针。
    vector<void*> blocks;   // vector 是 C++ 标准库中的一个动态数组容器，用于存储和管理一系列元素。
    // 在这里，blocks 是一个 vector，存储了每次从系统申请的大块内存的指针。
    // 每当内存池需要更多内存时，它会向系统申请一大块内存，并将该块内存的指针保存在 blocks 中，以便在析构函数中统一释放这些内存块。
    // 常用api有 push_back() 用于向 vector 中添加元素，size() 用于获取 vector 中元素的数量，clear() 用于清空 vector 中的所有元素等。

    // blockSize 是每次分配给用户的小块大小，单位是字节。
    size_t blockSize;   // size_t 是一个无符号整数类型，通常用于表示内存大小或数组索引等非负值。它的大小取决于平台，通常是32位或64位系统上为4字节或8字节。

    // blocksPerChunk 是每次申请一大块内存时，包含的小块数量。
    size_t blocksPerChunk;

    // currentBlock 指向当前正在使用的那块大内存。
    // char* currentBlock; 这里使用 char*，是因为它表示“字节指针”。
    // char 大小正好是 1 字节
    // char* 允许按字节偏移地址
    // 内存池要做“原始内存管理”，不关心里面放的是什么类型，只要按字节分配即可
    char* currentBlock;

    // currentPosition 表示 currentBlock 中已经使用了多少字节。
    size_t currentPosition;

public:
    // 构造函数：size 表示每个小块的大小，perChunk 表示每次分配的大块包含多少个小块。
    // perChunk = 1024 只是给 MemoryPool 的构造函数设定了一个默认值
    MemoryPool(size_t size, size_t perChunk = 1024)
    : blockSize(size), blocksPerChunk(perChunk), currentBlock(nullptr), currentPosition(0) {
        // 一开始就分配一个大块内存，方便后面直接分配小块使用。
        allocateChunk();
        cout << "1 MemoryPool created with block size " << size << " bytes and " << perChunk << " blocks per chunk." << endl;
    }

    // 析构函数：释放所有之前申请的大块内存。
    ~MemoryPool() {
        for (void* block : blocks) {
            cout << "2 Releasing memory block at " << block << endl;
            // 由于 blocks 中存储的是 void* 类型的指针，直接使用 delete
            // static_cast是C++中的一种类型转换操作符，用于在编译时进行类型转换。
            // 它比C风格的强制类型转换更安全，因为它会检查类型之间的兼容性。
            // void* 是“无类型指针”，不能直接用 delete[]
            // 作用是把 void* 指针转换成 char* 指针，然后再用 delete[] 释放内存。
            delete[] static_cast<char*>(block);
        }
    }

    // allocate() 用于从内存池中分配一块小内存。
    void* allocate() {
        cout << "3 Allocating " << blockSize << " bytes from memory pool." << endl;
        // 如果当前大块剩余空间不足以分配一个新的小块，就申请新的大块。
        // 总大小 = blocksPerChunk * blockSize。
        // currentPosition + blockSize 是下一次分配后的位置，如果它超过了当前大块的总大小，就需要申请新的大块。
        if (currentPosition + blockSize > blocksPerChunk * blockSize) {
            cout << "4 Current block exhausted, allocating new chunk." << endl;
            allocateChunk();
        }

        // 返回当前可用位置的指针。
        void* result = currentBlock + currentPosition;

        // 更新当前位置，下一次分配时从下一个小块开始。
        currentPosition += blockSize;
        cout << "5 Allocated at address " << result << ", current position in block: " << currentPosition << " bytes." << endl;

        return result;  // result 是一个 void* 指针，指向当前大块内存中分配给用户的小块的起始位置。
        // 用户可以把这个指针转换成他们需要的类型来使用。
    }

    // deallocate() 这里不做实际回收，保持简单。
    // 真正的内存池实现可能会把这块小内存放回空闲列表。
    void deallocate(void* ptr) {
        cout << "6 Deallocate called for address " << ptr << ", but this simple memory pool does not support deallocation." << endl;
        (void)ptr; // 它把 ptr 转成 void，实际上不做任何操作，这样写的目的是避免“未使用参数”的编译警告
    }

private:
    // allocateChunk() 申请一个新的大块内存，并初始化内部状态。
    void allocateChunk() {
        cout << "7 Allocating new memory chunk of size " << (blocksPerChunk * blockSize) << " bytes." << endl;
        // 申请一块大内存，它的大小等于 blocksPerChunk * blockSize。
        char* newBlock = new char[blocksPerChunk * blockSize];

        // 把这块大内存保存起来，便于最后统一释放。
        blocks.push_back(newBlock);

        // 当前使用的内存块指针改为这块新内存，当前偏移归零。
        currentBlock = newBlock;
        currentPosition = 0;
    }
};

// 测试类，用来演示如何在内存池上构造对象。
class TestClass {
private:
    int value;

public:
    // 构造函数，把传入的值保存到成员变量中。
    TestClass(int v) : value(v) {
        cout << "TestClass constructed with value " << value << endl;
    }

    // 析构函数，销毁对象时打印信息。
    ~TestClass() {
        cout << "TestClass destructed with value " << value << endl;
    }

    // 返回对象保存的值。
    int getValue() const {
        return value;
    }
};

int main() {
    // 创建一个内存池，块大小为 TestClass 的大小。
    cout << "main: Creating memory pool for TestClass objects." << endl;
    MemoryPool pool(sizeof(TestClass));
    cout << "main: Memory pool created." << endl;

    cout << "------------------------------" << endl;

    // 在内存池中分配空间并直接构造对象。
    // 这里使用的是“定位 new”，它不会分配内存，而是在已有内存上构造对象。
    TestClass* obj1 = new (pool.allocate()) TestClass(1);
    cout << "main: Object 1 created." << endl;
    TestClass* obj2 = new (pool.allocate()) TestClass(2);
    cout << "main: Object 2 created." << endl;
    TestClass* obj3 = new (pool.allocate()) TestClass(3);
    cout << "main: Object 3 created." << endl;

    cout << "------------------------------" << endl;

    // 输出对象的值。
    cout << "main: obj1 value: " << obj1->getValue() << endl;
    cout << "main: obj2 value: " << obj2->getValue() << endl;
    cout << "main: obj3 value: " << obj3->getValue() << endl;

    // 因为我们使用的是内存池分配的内存，所以对象的析构必须手动调用。
    obj1->~TestClass();
    obj2->~TestClass();
    obj3->~TestClass();

    // 程序结束后，MemoryPool 的析构函数会释放所有申请的大块内存。
    return 0;
}
