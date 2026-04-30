/*
 * MemoryPool.cpp —— 与 include/MemoryPool.h 配套的实现翻译单元
 *
 * 作用：在此文件中写出 MemoryPool / HashBucket 的成员函数定义（函数体）。
 * 为什么单独放在 .cpp：若把带函数体的定义写进被广泛 #include 的 .h，多个 .cpp 会各自编译出一份定义，
 *      链接阶段常出现 multiple definition（违反 ODR：One Definition Rule）。
 */

/* #include "路径"：预处理指令，把 MemoryPool.h 的内容文本插入此处；
 *  "../include/MemoryPool.h"：相对路径，从本文件所在目录（src）向上一级再找 include。
 *  必须先包含声明头文件，下面的 MemoryPool::xxx 定义才能与类声明匹配。 */
#include "../include/MemoryPool.h"

/* namespace 名字 { ... }：命名空间定义语法；块内符号的全名默认为 Kama_memoryPool::符号，
 * 与头文件中声明处在同一命名空间，才能实现「类外定义成员函数」：返回类型 类名::函数名(参数) { } */
namespace Kama_memoryPool
{

/* ---------- MemoryPool 构造函数 ---------- *
 * 语法：类外定义构造函数写作「类名::类名(形参列表) : 成员初始化列表 { 函数体可为空 }」。
 * - 构造函数名必须与类名相同；不得写返回类型（含 void）。
 * - 初始化列表「:」后用逗号分隔「成员名(初值)」，在进入 { } 之前完成成员初始化（优于在 {} 内赋值）。
 */
MemoryPool::MemoryPool(size_t BlockSize)
    : BlockSize_(BlockSize)              // int BlockSize_ ← 形参 BlockSize：大块字节数
    , SlotSize_(0)                       // int SlotSize_：槽宽尚未 init，置 0 表示未就绪
    , firstBlock_(nullptr)               // Slot*：块链表头；尚无块则为空指针
    , curSlot_(nullptr)                  // Slot*：顺序分配游标；尚无可用槽则为空
    , freeList_(nullptr)                 // atomic 包装的链表头：原子变量的初始指针状态为空
    , lastSlot_(nullptr)                 // Slot*：当前物理连续区间内「末尾合法槽」标记；尚无则为空
{
    /* 构造函数体为空：所需初始化均在初始化列表完成。 */
}

/* ---------- MemoryPool 析构函数 ---------- *
 * 语法：析构函数写作「类名::~类名()」，无参数、无返回类型。
 * while (条件)：循环；指针可作为条件——空指针为假，非空为真。
 */
MemoryPool::~MemoryPool()
{
    Slot* cur = firstBlock_;             // 从第一块内存对应的链表结点出发遍历

    while (cur)                          // cur 非 nullptr 则继续释放后续块
    {
        /* Slot::next 为 std::atomic<Slot*>：原子指针；此处语境需读到「裸指针 Slot*」参与遍历，
           具体写法以实现为准（标准写法常为 cur->next.load(...)）。保存下一结点地址后再释放当前块。 */
        Slot* next = cur->next;

        /* operator delete(void*)：全局释放运算符；释放此前 operator new 分配的存储，不调用对象析构。
           reinterpret_cast<void*>(cur)：把 Slot* 转成 void*，满足 delete 形参类型约定。
           reinterpret_cast 是 C++ 最暴力、最底层的强制类型转换，直接把一段内存的二进制数据，重新解释成另一种类型，
           不做任何安全检查、不改变数据本身，只改变编译器怎么 “看” 这段数据。 
           reinterpret音标：/ˌriːɪnˈtɜːprət/含义：重新解释、重新解读；cast音标：/kɑːst/含义：类型转换、投射。
        */
        operator delete(reinterpret_cast<void*>(cur));

        cur = next;                       // 移动到链表中下一块（逻辑上的「下一块」指针）
    }
}

/* ---------- init：设定槽宽并重置内部指针 ---------- *
 * assert(expr)：断言宏（通常调试构建启用）；expr 为假则中止程序，提示前置条件被破坏。
 */
void MemoryPool::init(size_t size)
{
    assert(size > 0);                    // 槽宽必须为正，否则后续除法/对齐无意义

    SlotSize_ = size;                    // 记录本池单个槽的字节粒度（应为 8 的倍数档）

    firstBlock_ = nullptr;   // 块链表头指针，指向首块内存。这里设为 nullptr，表示池内还没有任何分配块（链表重置到无）。
    curSlot_    = nullptr;   // 当前用于顺序分配的槽起始指针。赋为 nullptr 表示没有可分配的槽，需要重新分配新块后才能使用。
    freeList_   = nullptr;   // 空闲槽无锁链表的原子头指针。复位为 nullptr，表示当前没有可供复用的空闲槽。
    lastSlot_   = nullptr;   // 当前可分配块的末尾槽指针，用于判断是否需要申请新块。这里重置为 nullptr，须在重新分配新块后再设置。
}


/* ---------- allocate：分配一块槽内存 ---------- *
 * void*：返回「未带类型」的指针；调用方可 reinterpret_cast 成所需类型。
 * lock_guard<mutex> lock(mutex_)：RAII 锁；对象构造时加锁，离开包围它的作用域析构时自动解锁。
 * RAII = Resource Acquisition Is Initialization
 * 
 * 中文直译：资源获取即初始化。
 * RAII 锁 就是：用 RAII 思想封装的互斥锁
 * C++ 里典型：
 * std::lock_guard
 * std::unique_lock
 * 工作逻辑
 * 定义锁对象时构造函数自动加锁
 * 出作用域、对象销毁时析构函数自动解锁
 * 不用自己手动 lock ()、unlock ()，杜绝忘解锁、异常漏解锁
 * curSlot_ += n：指针算术；Slot* 前进 n 个 Slot 相隔的字节数为 n*sizeof(Slot)，此处用 SlotSize_/sizeof(Slot)
 *                作为步长，是实现策略：把槽宽对齐到 Slot 指针步进的等价换算（依赖内存布局约定）。
 */
void* MemoryPool::allocate()
{
    Slot* slot = popFreeList();          // ① 优先尝试从无锁空闲链表弹出一个已回收槽

    if (slot != nullptr)                 // if (指针)：非空表示成功拿到复用槽
        return slot;                     // 直接返回这块内存给用户侧当作 void* 使用（实为 Slot*）

    Slot* temp;                          // 存放本轮顺序切分得到的槽指针

    {                                    // 额外一对 { }：限定 lock_guard 生命周期（只在扩块/推进游标期间持锁）
        std::lock_guard<std::mutex> lock(mutexForBlock_);   // 这里构造 lock_guard → 加锁

        if (curSlot_ >= lastSlot_)       // 游标越过末尾标记 → 当前大块无可切槽，需要申请新大块
        {
            allocateNewBlock();          // 向系统申请 BlockSize_ 字节并更新 curSlot_/lastSlot_/链表头
        }

        temp = curSlot_;                 // 记下当前可用槽起始地址

        curSlot_ += SlotSize_ / sizeof(Slot);  // 游标前进「一个用户槽」对应的 Slot* 偏移（实现细节）
    }                                    // 这里析构 lock_guard → 释放互斥锁

    return temp;                         // 返回新切分出的槽指针
}

/* ---------- deallocate：归还槽 ---------- *
 * if (!ptr)：! 取反；ptr 为空则直接返回，避免对空指针 reinterpret/push。
 */
void MemoryPool::deallocate(void* ptr)
{
    if (!ptr)   // 如果 ptr 为空，直接返回，避免对空指针 reinterpret/push
        return;

    /* reinterpret_cast<Slot*>(ptr)：把用户归还的 void* 视为 Slot*（与 allocate 返回时约定一致）。 */
    Slot* slot = reinterpret_cast<Slot*>(ptr);

    pushFreeList(slot);                  // 无锁地把结点推回空闲栈，供下次 allocate 复用
}

/* ---------- allocateNewBlock：申请新的大块并接入链表 ---------- *
 * operator new(size_t)：分配至少 size 字节原始存储；不对对象调用构造函数。
 * reinterpret_cast：在不同指针类型间「不重译比特」地转换含义（程序员保证合法）。
 */
void MemoryPool::allocateNewBlock()
{
    // 使用全局的 operator new 函数，向操作系统（堆）请求分配一块原始内存，大小为 BlockSize_ 字节。
    // operator new 就是 C++ 版的 malloc，只负责分配裸内存。
    // 返回值是 void* 指针，指向新分配的大块内存的起始地址。
    // 注意：这里分配的是“裸内存”，不会执行任何构造函数、类型转换，仅仅是分配出一块可以任意用途的内存区域。
    void* newBlock = operator new(BlockSize_);

    /* 约定：newBlock 起始处留出一块「相当于 Slot* next」大小的头部用于串联块链表；
       写入 next = 旧的头结点 firstBlock_，再把 firstBlock_ 更新为新块（头插法）。 */
    reinterpret_cast<Slot*>(newBlock)->next = firstBlock_;
    firstBlock_ = reinterpret_cast<Slot*>(newBlock);

    /* char* body：跳到头部之后的区域，作为「可对齐后再切槽」的用户内存区起始字节地址。
       sizeof(Slot*)：此处实现按「指针宽度」跳过头部（注意：若 Slot 实际更大则需与头文件布局一致）。 */
    char* body = reinterpret_cast<char*>(newBlock) + sizeof(Slot*);

    // paddingSize: padding size，对齐填充字节数的全拼是 "padding size"
    // padPointer: pad pointer，对齐指针的全拼是 "pad pointer"
    size_t paddingSize = padPointer(body, SlotSize_);   // 计算 body 起始相对 SlotSize_ 对齐需要的填充字节数（padding size）；padPointer 即 pad pointer，计算对齐填充

    // curSlot_ 表示当前可分配槽（Slot）的起始指针，用于顺序分配内存。
    // 这里将 body（实际可以分配用户数据的区域起点）加上对齐所需的填充字节（paddingSize），
    // 确保 curSlot_ 按 SlotSize_ 字节对齐，满足不同类型数据的对齐要求（避免未对齐带来的访问性能或崩溃问题）。
    // 然后用 reinterpret_cast 将结果转换为 Slot* 指针类型，方便后续以 Slot 粒度操作池内存。
    curSlot_ = reinterpret_cast<Slot*>(body + paddingSize);

    /* lastSlot_：用指针算术大体框定「本块内最后一个可起始槽」的位置（实现细节；与 SlotSize_/BlockSize_ 配合）。
       reinterpret_cast<size_t>(newBlock)：先把块起始地址转成整数再做字节偏移加减。 */
    // 将 lastSlot_ 设置为本块（Block）的最后一个可用槽的起始指针
    // 作用：
    //  - reinterpret_cast<size_t>(newBlock)：把新分配的大块内存的起始地址转换为整数，方便指针的字节加减法。
    //  - BlockSize_：表示本块总共分配的字节数。
    //  - SlotSize_：表示每一个槽占据的字节数（用户实际需要的块大小）。
    //  - Block 的尾部能容纳的最后一个完整槽的起始地址是 newBlock + BlockSize_ - SlotSize_
    //  - +1：一般用于使 lastSlot_ 指向「最后一个合法槽的下一个地址」（实现细节，便于分配逻辑统一判断）
    //  - 最终结果再 reinterpret_cast<Slot*> 转为 Slot* 指针型，方便后续指针比较和迭代
    lastSlot_ = reinterpret_cast<Slot*>(
        reinterpret_cast<size_t>(newBlock)  // 块起始地址转整型
        + BlockSize_                        // 加上整块长度——到 Block 结尾
        - SlotSize_                         // 倒回一个完整槽的长度——定位到最后一个完整槽起点
        + 1                                 // 指向最后可用槽的下一个字节（实现细节，通常用作区间上界便于判定边界）
    );
}

/* ---------- padPointer：计算对齐填充 ---------- *
 * %：取余；若 rem==0 表示已对齐，返回 0；否则返回还需跳过多少字节才能对齐到 align 边界。
 */
/* 
 * padPointer：用于计算从指针 p 起始，到下一个 align 对齐边界还需要跳过多少个字节（即 padding size）。
 * 典型用于内存池、对齐场景，保证数据结构满足平台的数据对齐要求，提升访问效率、规避未对齐引发的潜在异常。
 *
 * 参数说明：
 *   char* p   ：指向起始位置的裸指针，一般为分配内存块中的某个字节地址；
 *   size_t align：希望对齐的字节数（通常为每个对象的大小或特定对齐需求）。
 *
 * 返回值说明：
 *   返回从 p 到最近的 align 对齐的起始地址，所需跳过的字节数（若 p 已经对齐，则=0）。
 */
size_t MemoryPool::padPointer(char* p, size_t align)
{
    // reinterpret_cast<size_t>(p)：将指针类型强制转换为整数类型，便于做字节加减和取模运算。
    // % align：取余操作，计算 p 已经偏离对齐边界的字节数（rem == 0 表示已经对齐）。
    size_t rem = reinterpret_cast<size_t>(p) % align;
    
    // 条件运算（三元运算）:
    //   如果 rem == 0，说明 p 已经对齐，无需填充，返回 0；
    //   否则，距离下一个 align 倍数还差 (align - rem) 个字节，需要补齐。
    return (rem == 0) ? 0 : (align - rem);
}

/* ---------- pushFreeList：无锁空闲栈 push（Treiber stack 思路） ---------- *
 * while (true)：无限循环直到 CAS 成功返回。
 * load(memory_order_relaxed)：原子加载；relaxed 最弱顺序，适合仅与本 CAS 搭配的无竞争优化路径。
 * store(..., relaxed)：原子写入。
 * compare_exchange_weak(expected, desired, succ, fail)：若原子变量当前值等于 expected，则改为 desired 并成功；
 *      weak 版本允许伪失败（spurious failure），常在循环里重试。
 */
bool MemoryPool::pushFreeList(Slot* slot)
{
    // pushFreeList：将空闲 slot 无锁推入空闲栈（Treiber stack 无锁实现）。
    while (true)  // 无限循环，直到成功将新结点推入栈顶
    {
        // 第一步：原子读取当前空闲链表的头指针快照（即当前栈顶）
        // 原子体现在：freeList_ 是 std::atomic<Slot*> 类型，load 是原子操作，保证读取的值是当前最新的。
        Slot* oldHead = freeList_.load(std::memory_order_relaxed);

        // 第二步：将新 slot 的 next 指针指向当前头，准备接入链表
        // 原子体现在：slot->next.store 是原子操作，保证写入的值是当前最新的。
        slot->next.store(oldHead, std::memory_order_relaxed);

        // 第三步：CAS（比较并交换），尝试把链表头原子性地切换为 slot
        // 参数含义：
        //   oldHead：期待原值
        //   slot   ：新值（替换为新结点）
        //   memory_order_release：确保之前的写操作（如 slot->next）在其它线程可见
        //   memory_order_relaxed ：失败时无需额外同步约束
        // 若 freeList_ 仍等于 oldHead，则替换为 slot，操作成功，函数返回 true
        // 如期间有其它线程竞争，CAS 失败，则进入下一轮重试
        if (freeList_.compare_exchange_weak(
                oldHead,
                slot,
                std::memory_order_release,
                std::memory_order_relaxed))
        {
            // 推入栈顶成功，返回 true
            return true;
        }
        // CAS 失败（有竞争），循环重试直到成功为止
    }
}

/* ---------- popFreeList：无锁空闲栈 pop ---------- *
 * memory_order_acquire：获取语义；常与 push 侧的 release 配对，建立「 publishes-before 」可见性关系。
 * try/catch(...)：捕获任意异常；此处防御 next.load 路径上的异常（风格少见），失败则 continue 重试。
 */
Slot* MemoryPool::popFreeList()
{
    while (true)
    {
        Slot* oldHead = freeList_.load(std::memory_order_acquire); // 读取栈顶

        if (oldHead == nullptr)          // 空栈：无空闲结点可分配
            return nullptr;

        Slot* newHead = nullptr;

        try                              // try { } catch：异常处理语法；此处包围可能抛出异常的读取逻辑
        {
            newHead = oldHead->next.load(std::memory_order_relaxed); // 读取第二个结点指针作为新栈顶候选
        }
        catch (...)                      // ...：捕获所有异常类型
        {
            continue;                    // 读出失败则重试整套 CAS 流程
        }

        /* 若栈顶仍为 oldHead，则把栈顶原子更新为 newHead（弹出成功）；否则重试。 */
        if (freeList_.compare_exchange_weak(oldHead, newHead,
                std::memory_order_acquire, std::memory_order_relaxed))
        {
            return oldHead;              // 返回弹出的旧头结点给用户当作空闲槽
        }
    }
}

/* ---------- HashBucket::initMemoryPool：初始化全局各档位池 ---------- *
 * for (init; condition; increment)：经典 for 循环语法。
 */
/* 
 * HashBucket::initMemoryPool
 * 作用：初始化全局所有内存池分档（如每 8 字节为一档），为每个分档的 MemoryPool 实例设置对应的槽宽。
 * 注释说明：
 * - 这是一个静态成员函数，只需在程序启动时调用一次（通常在 main 前或首次分配前）。
 * - 遍历所有分档（如 8B, 16B, ..., N*8B），依次初始化每个分档的 MemoryPool 对象，其分配粒度即 (i+1)*8 字节。
 * - 为后续 useMemory/freeMemory 根据对象大小路由到合适的 MemoryPool 单例做准备，提高分配效率、减少碎片。
 */
void HashBucket::initMemoryPool()
{
    for (int i = 0; i < MEMORY_POOL_NUM; i++)
    {
        /* getMemoryPool(i)：拿到第 i 个静态 MemoryPool 引用；init 槽宽为 (i+1)*8 字节档。 */
        getMemoryPool(i).init((i + 1) * SLOT_BASE_SIZE);
    }
}

/* ---------- HashBucket::getMemoryPool：返回档位对应的单例池 ---------- *
 * static MemoryPool memoryPool[]：函数内静态局部变量——首次进入函数时初始化一次，生命周期持续到程序结束；
 * MemoryPool&：返回引用语法；避免拷贝 MemoryPool（拷贝可能禁用或不期望）。
 */
MemoryPool& HashBucket::getMemoryPool(int index)
{
    static MemoryPool memoryPool[MEMORY_POOL_NUM]; // MEMORY_POOL_NUM 个池组成的静态数组（全局一次初始化）

    return memoryPool[index];            // 按下标返回某个池的可修改引用（allocate/deallocate 将作用其上）
}

} /* namespace Kama_memoryPool —— 命名空间结束 */
