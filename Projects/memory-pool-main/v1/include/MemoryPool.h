#pragma once // 头文件只编译一次，等价于传统 include guard，避免重复定义

/*
 * 【工程习惯】实际开发里 .h / 头文件一般写什么、怎么组织
 *
 * 头文件的作用：给多个 .cpp「同一份声明/接口」，让编译器知道类型与函数签名；链接阶段再到 .obj 里找实现。
 *
 * 适合放在 .h 里：
 * - 类型定义：class / struct / enum / enum class、using / typedef。
 * - 函数声明：返回类型、函数名、参数列表，末尾分号；实现在对应 .cpp（用户看到的是接口）。
 * - 类体内「默认 inline」的成员函数：短小、仅一两行的 getter/setter 等（仍在头文件也合法）。
 * - 模板与 constexpr：函数模板、类模板、需在用到处实例化的实现 → 通常必须写在头文件（或显式实例化少数几种 T）。
 * - 内联函数：显式 inline 的自由函数若定义在头文件中，可避免多重定义错误（ODR 允许多份相同定义）。
 * - 常量：优先 constexpr / const inline（C++17）替代宏；宏仍是遗留代码常见手段。
 * - 最小必要 #include：头文件里包含「声明所依赖」的类型；能用前置声明（class Foo;）就少包含，缩短编译时间。
 *
 * 谨慎或避免：
 * - 非 inline、非模板的函数「定义」长期堆在 .h → 多个 .cpp 包含会得到重复定义链接错误；应挪到单个 .cpp。
 * - 在头文件里定义带静态存储期的变量（如 int x = 1;）→ 每个包含单元各有一份；改用 inline 变量或 extern + 某 .cpp 定义。
 * - #include 泛滥：能前置声明就不要把整个重量级头拉进来；复杂私有成员可考虑 PIMPL（实现细节挪到 .cpp）。
 *
 * 与本文件的关系：下面 MemoryPool 的大部分成员函数声明在 .h，实现在 MemoryPool.cpp；
 * HashBucket::useMemory/freeMemory、newElement/deleteElement 等因简单或模板原因写在头里——属于常见折中。
 */

/*
 * MemoryPool.h —— Projects/memory-pool-main/v1 的核心头文件
 *
 * 整个文件在 v1 里干什么：
 * - 声明（并把少量逻辑内联写在本文件）一整套「小块内存池」：按尺寸分档、池内复用、大块走系统堆。
 * - 具体成员函数的实现主要在同级目录 ../src/MemoryPool.cpp；测试与其它代码只需 #include 本头文件并链接该实现。
 *
 * 三块职责（从上到下阅读即可对照代码）：
 * 1) Slot + MemoryPool：某一固定槽宽（8 字节档位之一）的分配器；向系统按 BlockSize 申请大块，
 *    切成槽；allocate 优先从无锁空闲链表取，否则顺序切分；deallocate 挂回链表；超过当前块则扩块（扩块路径互斥）。
 * 2) HashBucket：门面（静态接口）。按请求的 size 把 useMemory/freeMemory 路由到 MEMORY_POOL_NUM 档池中之一；
 *    size 大于 MAX_SLOT_SIZE 则用全局 operator new/delete，避免巨型对象占用固定档位。
 * 3) newElement / deleteElement（模板）：在 HashBucket::useMemory 给出的内存上用 placement new 构造 T，
 *    deleteElement 先 ~T() 再 freeMemory；语义接近 new/delete，但底层走池。
 *
 * 典型用法：进程/线程开始使用池前调用 HashBucket::initMemoryPool()；
 *          分配 small 对象用 newElement<T>(...) / deleteElement(p)，或直接 useMemory + placement new（自担类型责任）。
 */

#include <atomic>   // std::atomic：无锁链表结点指针的原子读写（多线程安全）
#include <cassert>  // assert：调试期断言，全称：C assertion，中文：C 断言（调试检查）
#include <cstdint>  // 固定宽度整数类型（本文件中若未直接使用可保留为扩展预留），全称：C standard integer，中文：C 标准固定宽度整数类型
#include <iostream> // 标准 IO（若实现中仅调试输出会用到）
#include <memory>   // 智能指针等标准内存工具
#include <mutex>    // std::mutex：保护大块分配路径，避免多线程重复申请整块内存，全称：mutual exclusion，中文：互斥锁

namespace Kama_memoryPool   // 命名空间：Kama_memoryPool，全称：Kama memory pool，中文：Kama 内存池
{
/* ---------- 编译期常量：内存池分层策略 ---------- */
#define MEMORY_POOL_NUM 64      // 内存池个数：按槽大小从 8 字节起共分 64 档（8,16,...,512）
#define SLOT_BASE_SIZE 8        // 槽对齐基数：所有槽大小均为 8 的倍数，便于索引计算
#define MAX_SLOT_SIZE 512       // 走内存池的最大分配单元：超过则用全局 operator new/delete

/* ---------- 空闲链表结点 Slot ---------- */
/* 说明：具体「槽」的用户可见大小由 MemoryPool::SlotSize_ 决定，每个池不同。
   这里的 Slot 只表示空闲链表结点：仅存一个 next 指针。
   因此 sizeof(Slot) 并不是「一整块给用户用的槽」的字节数，实际槽更大。 */
struct Slot 
{
    /* std:: ：标准库命名空间前缀，表明其后符号来自 C++ 标准库。
       atomic ：原子类型模板；对该指针的加载、存储、compare_exchange 等为原子操作，
               多线程下可避免数据竞争，常与 CAS 配合实现无锁空闲链表（push/pop）。
       Slot* ：模板实参，表示「指向 Slot 的指针」；空闲链表里每个结点通过指针指向下一结点。
       next  ：成员变量名；语义为链表中的「后继指针」，保存下一个空闲 Slot 的地址。 */
    std::atomic<Slot*> next;
};

/* ---------- MemoryPool：单一槽尺寸的块链表 + 空闲栈 ---------- */
class MemoryPool
{
public:
    /* public：访问说明符，其后成员/方法可被类外部调用（如 HashBucket 使用 allocate/deallocate）。 */

    /* MemoryPool(size_t BlockSize = 4096)：构造函数，与类同名且无返回类型。
       size_t：无符号整数类型，表示字节数等非负尺度。
       BlockSize：形参名，表示单次向系统申请的「大块」尺寸；库内用于 allocateNewBlock。
       = 4096：默认实参，省略该参数时一块为 4096 字节（常见一页大小）；再大可减少系统调用次数。
       作用：创建对象并初始化池状态；具体槽宽通常由 init 再设定。 */
    MemoryPool(size_t BlockSize = 4096);

    /* ~MemoryPool()：析构函数；~ 为析构记号，无参数、无返回类型。
       作用：对象销毁时释放本池持有的所有大块内存，避免泄漏。 */
    ~MemoryPool();

    /* void init(size_t)：void 表示无返回值；size_t 参数一般为单个槽的字节宽 SlotSize_。
       作用：在首次 allocate 前设定「每个槽多大」，与 BlockSize_ 配合决定每块能切多少槽。 
       函数原型：我有一个叫 init 的函数，它接收一个 size_t 类型的参数，但我现在不想给这个参数起名字。*/
    void init(size_t);

    /* void* allocate()：返回 void*（未类型化的裸指针），调用方再转换为所需类型。
       作用：分配一块至少 SlotSize_ 字节的内存；优先从空闲链表取，否则从新大块顺序切分或扩块。 */
    void* allocate();

    /* void deallocate(void*)：void* 形参表示任意类型的指针，此处应为此前本池 allocate 返回的地址。
       作用：把该块视为 Slot 结点挂回空闲链表（pushFreeList），供后续 allocate 复用。 */
    void deallocate(void*);

private:
    /* private：仅类内部与友元可访问；以下为内部算法与无锁链表操作，对外隐藏实现细节。 */

    /* void allocateNewBlock()：无参数；当前大块用尽或尚未分配时调用。
       作用：按 BlockSize_ 再申请一块内存，接入块链表，并更新 curSlot_/lastSlot_ 等游标。 */
    void allocateNewBlock();

    /* padPointer(char* p, size_t align)：返回 size_t，为从地址 p 起对齐到 align 的倍数所需「填充」字节数（已对齐则返回 0）。
       char* p：块内某起始字节地址；size_t align：对齐模数（实现中与 SlotSize_ 一致）。
       作用：计算偏移使后续槽起始满足对齐分配要求。 */
    size_t padPointer(char* p, size_t align);

    /* bool pushFreeList(Slot* slot)：返回 bool 表示 CAS/入栈是否成功。
       Slot* slot：指向待挂回空闲链表的结点。
       作用：无锁地把结点推到全局空闲链表头（常见于 Treiber stack），供 popFreeList 取用。 */
    bool pushFreeList(Slot* slot);

    /* Slot* popFreeList()：返回指向一个空闲槽结点的指针，失败时可为 nullptr。
       作用：无锁地从空闲链表弹出一个结点；与 pushFreeList 成对维护复用队列。 */
    Slot* popFreeList();
private:
    int                 BlockSize_; // 每次系统级大块的大小（字节）
    int                 SlotSize_; // 本池单个槽的用户粒度（字节），为 8 的倍数档之一
    Slot*               firstBlock_; // 块链表头：指向第一块实际分配的内存（多块时用链表串联）
    Slot*               curSlot_; // 顺序分配游标：从未使用过的槽中「依次切」时使用
    std::atomic<Slot*>  freeList_; // 空闲链表头：曾被 allocate 后又 deallocate 的槽由此复用
    Slot*               lastSlot_; // 当前这块物理连续内存里最后一个合法槽位置（超过则需新块）
    //std::mutex          mutexForFreeList_; // （可选）若改为互斥保护空闲链表则启用；当前用原子 CAS 则不需要
    std::mutex          mutexForBlock_; // 扩展新块时加锁，避免多线程同时 allocateNewBlock 造成重复浪费
};

/* ---------- HashBucket：按请求字节大小路由到多档 MemoryPool（命名类比哈希桶分段） ---------- */
class HashBucket /* class：定义类型；HashBucket 为本项目中集中管理「按尺码选用内存池」的门面 */
{
public: /* 公有：外部通过静态函数分配/释放，无需实例化 HashBucket 对象 */

    /* static void initMemoryPool()：
       static：成员属于类而非某个对象，用 HashBucket::initMemoryPool() 调用，无 this 指针。
       void：无返回值；作用：一次性构造并初始化 MEMORY_POOL_NUM 个 MemoryPool（各档 SlotSize 不同）。 */
    static void initMemoryPool();

    /* static MemoryPool& getMemoryPool(int index)：
       MemoryPool&：返回引用，避免拷贝 MemoryPool；调用者可在其上连续调用 allocate/deallocate。
       int index：池下标，合法范围一般为 [0, MEMORY_POOL_NUM)；与「第几档 8 字节槽」对应。
       作用：按档位取出唯一的全局（或静态存储）MemoryPool 实例。 */
    static MemoryPool& getMemoryPool(int index);

    /* useMemory：按请求的原始字节数 size 决定走内存池还是全局堆（内联定义便于编译期优化）。 */
    static void* useMemory(size_t size)
    {
        /* size <= 0：非法请求，约定返回空指针，避免无意义分配 */
        if (size <= 0)
            return nullptr;
        /* size > MAX_SLOT_SIZE：超过池支持的最大槽（512），无法用固定档路由；
           operator new(size)：调用全局分配函数，语义与普通 new 底层分配一致（大块走系统堆）。 */
        if (size > MAX_SLOT_SIZE)
            return operator new(size);

        /* 路由公式：(size + 7) / SLOT_BASE_SIZE 等价于「除以 8 向上取整」，得到所需最小档是「几个 8 字节」；
           减 1 得到数组下标：例如 size∈[1,8]→档 1→index 0（对应 8 字节池）。
           getMemoryPool(...).allocate()：选中池并分配一块槽内存。 */
        return getMemoryPool(((size + 7) / SLOT_BASE_SIZE) - 1).allocate();
    }

    /* freeMemory：与 useMemory 对称；必须传入当初分配时一致的 size，才能回到正确档位（池不知道对象类型）。 */
    static void freeMemory(void* ptr, size_t size)
    {
        /* !ptr：空指针无需释放，防御性返回 */
        if (!ptr)
            return;
        /* 大块与 useMemory 中 operator new 配对，使用全局 operator delete 归还系统堆 */
        if (size > MAX_SLOT_SIZE)
        {
            operator delete(ptr);
            return;
        }

        /* 小对象：与分配时同一索引公式选池，再 deallocate 挂回该池的空闲链表 */
        getMemoryPool(((size + 7) / SLOT_BASE_SIZE) - 1).deallocate(ptr);
    }

    /* --- newElement 的友元前置声明（定义在同命名空间稍后位置）---
       template：其后声明的是函数模板而非单一函数。
       typename T：首个模板类型形参，表示要在池上构造的对象类型；编译期为每种 T 生成一份实例。
       typename... Args：模板类型参数包，对应构造函数的第 2～最后一个参数的类型（可变个数）。
       friend：将该模板的各实例声明为 HashBucket 的友元函数，可访问本类的 private/protected。
       T*：返回类型，指向已在缓冲区构造好的 T 对象。
       newElement：函数名；语义为「像 new 一样构造」，但底层走 HashBucket::useMemory。
       Args&&... args：函数形参包；Args&& 对每个实际参数为万能引用，便于 std::forward 完美转发到 T 的构造函数。 */
    template<typename T, typename... Args>
    friend T* newElement(Args&&... args);

    /* --- deleteElement 的友元前置声明 ---
       template<typename T>：仅依赖类型 T 的单参数模板（析构与释放只需知道对象类型以 sizeof(T) 还池）。
       friend void：友元；返回类型 void，表示不返回值。
       deleteElement：函数名；语义为先析构对象再将内存还回池。
       (T* p)：参数为指向待销毁对象的指针；非模板参数包，因删除接口固定为单指针。 */
    template<typename T>
    friend void deleteElement(T* p);
};

/* ---------- 命名空间内全局函数模板：在池分配的缓冲区内构造 / 析构 C++ 对象（用法类似 new/delete） ---------- */
template<typename T, typename... Args> /* 与 HashBucket 内 friend 声明匹配：T 为对象类型，Args... 为构造函数参数类型包 */
T* newElement(Args&&... args) /* T*：返回指向已构造对象的指针；Args&&...：万能引用形参包，保留左值/右值以转发 */
{
    T* p = nullptr; /* 初始化为空；后续仅在分配成功时写入有效地址 */

    /* useMemory(sizeof(T))：按对象体积选内存池档位并分配原始字节（未构造对象）。
       reinterpret_cast<T*>(...)：将 void* 解释为 T*；不负责类型安全，仅在本场景「紧接着 placement new」下成立。
       if ((p = ... ) != nullptr)：赋值并判断；分配失败（如池返回 nullptr）则跳过构造。 */
    if ((p = reinterpret_cast<T*>(HashBucket::useMemory(sizeof(T)))) != nullptr)
        /* new(p) T(...)：placement new，在已有存储 p 上调用 T 的构造函数，不再次分配堆。
           std::forward<Args>(args)...：将参数包按原值类别转发给匹配的 T 构造函数（完美转发）。 */
        new(p) T(std::forward<Args>(args)...);

    return p; /* 成功则为有效指针；失败或未进入 if 则为 nullptr */
}

template<typename T> /* 仅需类型 T：析构与释放大小都由 sizeof(T) 决定 */
void deleteElement(T* p) /* void：无返回值；T* p：指向先前由 newElement 创建的（或等价布局的）对象 */
{
    if (p) /* 空指针保护：对 nullptr 析构/释放为未定义行为 */
    {
        /* 显式析构：仅调用 ~T()（释放成员持有的资源）；不释放底层字节 */
        p->~T();

        /* freeMemory(ptr, sizeof(T))：第二参数须与分配时一致，才能路由回正确 MemoryPool。
           reinterpret_cast<void*>(p)：与 HashBucket::freeMemory 的 void* 形参一致；大块对象则在内部走 operator delete */
        HashBucket::freeMemory(reinterpret_cast<void*>(p), sizeof(T));
    }
}

} /* 闭合命名空间 Kama_memoryPool：其内符号（如 newElement）的全名为 Kama_memoryPool::newElement（或通过 using 引入） */
