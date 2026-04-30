# Memory Pool —— v1 版本说明（面向初学者）

本文面向「刚学完 C++ 基础、希望读懂一个小型内存池工程」的读者，概括 **v1** 的架构、设计取舍、关键流程与编译方式。**第 7 节**为与源码对齐的 **内存布局 ASCII 示意图**（变量、函数、指针算术）。实现细节仍以源码与注释为准：`include/MemoryPool.h`、`src/MemoryPool.cpp`、`tests/UnitTest.cpp`。

---

## 1. v1 是什么？解决什么问题？

### 1.1 背景

程序频繁 **`new` / `delete`**（或底层 **`malloc` / `free`**）时，会向操作系统反复申请、归还小块内存。每次跨越「用户态 ↔ 分配器 ↔ 内核/堆」的路径都有开销；小块多时，**碎片与缓存局部性**也会变差。

### 1.2 v1 的做法（一句话）

预先向系统按**较大块（Block）**申请内存，再在用户态切成固定**槽（Slot）**；小对象优先从**空闲链表**复用槽，减少进系统堆的次数。超过单槽上限的请求则退回 **`operator new` / `operator delete`**。

### 1.3 本版本特点

| 特性 | 说明 |
|------|------|
| 多档位 | 64 档，槽宽从 8B 到 512B（8 的倍数） |
| 路由 | 按请求 `size` 选池（`HashBucket` 命名类比「分桶」） |
| 并发 | 空闲栈用原子 + CAS（Treiber stack 思路）；**扩块 / 顺序切槽**用互斥锁保护 |
| C++ 对象 | `newElement` / `deleteElement`：池只给**裸字节**，用 **placement new** 构造、`~T()` 析构后再还池 |

---

## 2. 目录结构与构建

```
v1/
├── CMakeLists.txt      # CMake：把 src/*.cpp 与 tests/*.cpp 编成同一可执行文件
├── include/
│   └── MemoryPool.h    # 声明 + 少量内联（HashBucket::useMemory/freeMemory、newElement/deleteElement）
├── src/
│   └── MemoryPool.cpp  # MemoryPool / HashBucket 成员函数定义
├── tests/
│   └── UnitTest.cpp    # 简单 benchmark：内存池 vs 原生 new/delete
└── README.md           # 本文件
```

### 2.1 用 CMake 构建（推荐）

在 **`v1`** 目录下（非仓库根目录）：

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"   # 或 Visual Studio / Ninja 等
cmake --build .
./MemoryPoolProject             # Linux / MSYS
MemoryPoolProject.exe           # Windows 可执行文件名以生成器为准
```

`CMakeLists.txt` 要点：

- **`CMAKE_CXX_STANDARD 11`**：工程按 C++11 设定（可自行提高到 17/20，需验证）。
- **`file(GLOB SRC_FILES "src/*.cpp")`** 与 **`tests/*.cpp`**：合成**一个**可执行文件，因此 **`main` 只在测试里有一份**。
- **`-pthread`** / **`pthread`**：与 `<thread>` 测试相关。

### 2.2 不用 CMake：仅用 g++（注意「多个 .cpp」）

内存池实现分布在 **`MemoryPool.cpp`**，单独编译 **`UnitTest.cpp`** 会 **链接失败（undefined reference）**。

在 **`tests`** 目录示例：

```powershell
Set-Location "...\memory-pool-main\v1\tests"
g++ -std=c++11 -pthread -I../include ..\src\MemoryPool.cpp .\UnitTest.cpp -o UnitTest.exe
.\UnitTest.exe
```

PowerShell **不要使用** `cd /d`（那是 cmd 语法）。

### 2.3 控制台中文乱码（Windows）

若输出中文乱码，可先 **`chcp 65001`** 再运行程序，或使用 UTF-8 终端。

---

## 3. 架构分层（自上而下）

```
           ┌─────────────────────────────────────────┐
           │  应用 / 测试（UnitTest.cpp）              │
           │  newElement<T>() / deleteElement<T>()    │
           └─────────────────┬───────────────────────┘
                             │
           ┌─────────────────▼───────────────────────┐
           │  HashBucket（门面，静态接口）             │
           │  initMemoryPool / useMemory / freeMemory │
           └─────────────────┬───────────────────────┘
                             │ 按 size 选档 index
           ┌─────────────────▼───────────────────────┐
           │  MemoryPool × N（每档固定 SlotSize）      │
           │  allocate / deallocate                   │
           └─────────────────┬───────────────────────┘
                             │
           ┌─────────────────▼───────────────────────┐
           │  大块 Block（operator new）→ 切槽 / 空闲栈 │
           │  Slot（atomic next）参与无锁空闲链表        │
           └─────────────────────────────────────────┘
```

- **`HashBucket`**：不实例化也行；全是 **`static`** 方法，负责「要多少字节 → 用哪一个 **`MemoryPool`**」。
- **`MemoryPool`**：**一种槽宽**对应一个池实例；内部维护 **块链表**、**顺序分配游标**、**空闲链表头（原子）**。
- **`Slot`**：空闲链表里链表的结点类型；头文件里对 `sizeof(Slot)` 有说明——**不等于**用户槽的完整宽度，实现上复用结点里的 `next` 等约定与真实布局相关，读代码时对照 `allocateNewBlock`。

---

## 4. 设计思考（为什么这样设计）

### 4.1 为什么按 8 字节分档？

- 小对象尺寸离散，若每个尺寸一个池，池数量爆炸。
- **向上取整到 8 的倍数**（`(size+7)/8`）再映射到档位，用 **固定 64 档**覆盖 8～512，**实现简单、索引 O(1)**。
- 代价：可能为略小的对象分配略大的槽，有**内部分片（内部碎片）**，用空间换时间与实现复杂度。

### 4.2 为什么大于 512 走 `operator new`？

超大对象不适合塞进固定槽网格：浪费严重且档位不够用。超过 **`MAX_SLOT_SIZE`** 的请求交给全局 **`operator new/delete`**，与小块路径对称地在 **`freeMemory(ptr, size)`** 里分支。

### 4.3 为什么 allocate 优先空闲链表？

曾经被 `deallocate` 的槽可直接复用，避免反复「切新槽」和触碰 **`lastSlot_`**；热点路径更希望 **快**。空闲链表采用 **Treiber stack + CAS**，意在多线程 **`allocate/deallocate`** 时减少锁竞争（扩块仍加锁）。

### 4.4 为什么需要 `newElement` / `deleteElement`？

**池返回的是未构造的存储**。C++ 对象生命周期规则要求：**在那块内存上调用构造函数**才算合法对象；释放时要 **`~T()`** 再归还字节（除非 trivial destructor 的特殊讨论，初学阶段统一 **`deleteElement`** 最稳妥）。

对应语法：**placement new**，详见下文 API 小节。

---

## 5. 核心常量与路由公式（必读）

定义见 **`MemoryPool.h`**：

| 宏 | 含义 |
|----|------|
| `MEMORY_POOL_NUM` | 64 个池 |
| `SLOT_BASE_SIZE` | 8（档位基数） |
| `MAX_SLOT_SIZE` | 512（超过则走全局堆） |

**档位索引（池下标）**：

\[
\text{index} = \left\lceil \frac{\text{size}}{8} \right\rceil - 1 = \frac{\text{size} + 7}{8} - 1
\]

代码中即为：`((size + 7) / SLOT_BASE_SIZE) - 1`。

**释放时必须传入与分配时相同的 `size`**（通常 `sizeof(T)`），否则路由到错误的池会导致严重错误。

---

## 6. 流程分析

### 6.1 程序启动（测试）

1. **`HashBucket::initMemoryPool()`**  
   对 `i = 0 .. MEMORY_POOL_NUM-1`，调用 **`getMemoryPool(i).init((i+1)*SLOT_BASE_SIZE)`**，让每个池的槽宽为 8、16、…、512。
2. **`getMemoryPool`** 内部用 **`static MemoryPool memoryPool[]`**（函数内静态），保证全局只有一组池实例。

### 6.2 `useMemory(size)`（裸字节）

1. `size <= 0` → `nullptr`。  
2. `size > MAX_SLOT_SIZE` → **`operator new(size)`**。  
3. 否则 → **`getMemoryPool(index).allocate()`**。

### 6.3 `MemoryPool::allocate()`

1. **`popFreeList()`** 非空 → 直接返回该槽。  
2. 否则进入 **互斥区**（`mutexForBlock_`）：若 **`curSlot_ >= lastSlot_`**，**`allocateNewBlock()`**；再 **`curSlot_` 顺序推进**一个槽宽。  
3. 返回指针给上层。

### 6.4 `MemoryPool::deallocate(ptr)`

把 `ptr` 视为 **`Slot*`**，**`pushFreeList`** 挂回无锁空闲栈。

### 6.5 `newElement<T>(args...)`

1. **`p = useMemory(sizeof(T))`**；失败则返回 `nullptr`。  
2. **`new (p) T(std::forward<Args>(args)...)`** —— **placement new**，在已有存储上构造。  
3. 返回 **`T*`**。

### 6.6 `deleteElement<T>(p)`

1. 若 `p` 为空则返回。  
2. **`p->~T()`** 显式析构。  
3. **`freeMemory(p, sizeof(T))`** —— 与 **`useMemory(sizeof(T))`** 对称路由。

---

## 7. 内存布局手绘示意图（与 v1 源码变量、函数对齐）

下列示意均为 **ASCII 手绘风格**；地址增长方向从左到右、从下到上均已标明。行号指 **`MemoryPool.cpp` / `MemoryPool.h`** 当前 v1 版本，便于你对照打开。

---

### 7.1 `MemoryPool` 对象内部：数据成员放在哪、谁在用

声明见 **`MemoryPool.h`**（`class MemoryPool` 私有段）。逻辑上可将这些成员理解为「控制一整类槽宽的池」的状态机：

```
     ┌────────────────────────────────────────────────────────────────────┐
     │                     一个 MemoryPool 实例                           │
     │  （例如 HashBucket::getMemoryPool(i) 返回的 memoryPool[i]）         │
     ├─────────────────┬──────────────────────────────────────────────────┤
     │ BlockSize_      │ allocateNewBlock() 里 operator new(BlockSize_)    │
     │ int             │ 单次向系统申请的「大块」字节数（构造时默认 4096）    │
     ├─────────────────┼──────────────────────────────────────────────────┤
     │ SlotSize_       │ init(size) 写入；allocate() 里与 curSlot_ 步进、   │
     │ int             │ padPointer(body, SlotSize_)、lastSlot_ 计算相关   │
     ├─────────────────┼──────────────────────────────────────────────────┤
     │ firstBlock_     │ allocateNewBlock() 头插块链表；~MemoryPool() 遍历 │
     │ Slot*           │ 释放每一块 operator new 申请的存储                │
     ├─────────────────┼──────────────────────────────────────────────────┤
     │ curSlot_        │ 顺序分配：在「当前块槽区」里下一个可切出的槽起点   │
     │ Slot*           │ allocate() 在锁内可能与 allocateNewBlock() 联动   │
     ├─────────────────┼──────────────────────────────────────────────────┤
     │ freeList_       │ 无锁空闲栈的栈顶原子变量；pushFreeList/popFreeList  │
     │ atomic<Slot*>   │                                                   │
     ├─────────────────┼──────────────────────────────────────────────────┤
     │ lastSlot_       │ allocate() 判断 curSlot_ >= lastSlot_ 是否要新块   │
     │ Slot*           │ allocateNewBlock() 末尾根据 BlockSize_/SlotSize_ 设│
     ├─────────────────┼──────────────────────────────────────────────────┤
     │ mutexForBlock_  │ allocate() 中 std::lock_guard 保护扩块与 curSlot_  │
     │ std::mutex      │ 推进，避免多线程重复 allocateNewBlock              │
     └─────────────────┴────────────────────────────────────────────────────┘
```

**相关函数**：`MemoryPool::init` → 写 `SlotSize_` 并清空指针；`allocate` / `deallocate` / `allocateNewBlock` / `pushFreeList` / `popFreeList` / 析构函数共同读写上表字段。

---

### 7.2 向系统申请的「一整块 Block」：`allocateNewBlock()` 如何切

**函数**：`void MemoryPool::allocateNewBlock()`（`MemoryPool.cpp` 约 137–180 行）。

**步骤与变量（与源码一一对应）**：

| 变量 / 表达式 | 源码作用 |
|---------------|----------|
| `void* newBlock = operator new(BlockSize_)` | 向全局堆要 **`BlockSize_`** 字节连续裸内存，无构造 |
| `reinterpret_cast<Slot*>(newBlock)->next = firstBlock_` | 把块起始 **前几个字节** 当作可写 **`next`** 的结点：链到旧头 |
| `firstBlock_ = reinterpret_cast<Slot*>(newBlock)` | **头插**：`firstBlock_` 永远指向「最新申请」的块 |
| `char* body = (char*)newBlock + sizeof(Slot*)` | 块头只跳过 **`sizeof(Slot*)`** 宽（实现注释写「相当于串联块链的头部」） |
| `paddingSize = padPointer(body, SlotSize_)` | 从 `body` 起补到 **`SlotSize_` 对齐** 的填充字节数 |
| `curSlot_ = reinterpret_cast<Slot*>(body + paddingSize)` | **第一个可对用户顺序分配的槽**起点（对齐后） |
| `lastSlot_ = (Slot*)( (size_t)newBlock + BlockSize_ - SlotSize_ + 1 )` | 与 `curSlot_` 比较用于判断本块是否用尽；**`+1` 为当前实现细节** |

**一块内存内部（单 Block，侧视）**：

```
  低地址                                                                    高地址
  ───────────────────────────────────────────────────────────────────────────►

  ┌────────────────────┬──────────────────┬──────────────────────────────────┐
  │ 块头区域            │ padding          │ 槽区（每格用户概念上宽 SlotSize_）  │
  │ 长度 = sizeof(Slot*)│ 长度 = paddingSize│ 从 curSlot_ 起顺序切               │
  │                     │ （可能为 0）      │                                  │
  │ 被写成              │ padPointer(body, │           ...                     │
  │ Slot* 视角的 next   │  SlotSize_)      │                                  │
  │ 指向「更旧块的头」   │                  │ lastSlot_ 落在此块尾部附近（实现计  │
  │（头插链表）          │ body ──►         │ 算见源码）                         │
  └────────────────────┴──────────────────┴──────────────────────────────────┘
  ^
  newBlock（整块长度为 BlockSize_）
```

**说明（类型与实现的交界）**：头文件里 **`struct Slot`** 含 **`std::atomic<Slot*> next`**（体积通常等于指针宽度，但标准不保证与 `sizeof(Slot*)` 恒等）。**实现上块头跳过的是 `sizeof(Slot*)`**（见 `MemoryPool.cpp` 第 152 行）。阅读时以 **源码行的字节偏移** 为准。

#### 数值演算示例（与源码公式对齐：`BlockSize_=4096`，`SlotSize_=16`）

以下用 **`newBlock` 起始地址为 `base`**（相对偏移可把 **`base ≡ 0`**）；并假设常见 **LP64**：**`sizeof(Slot*) == 8`**，且 **`sizeof(Slot) == 8`**（本项目中 **`atomic<Slot*>`** 在许多编译目标下如此——请以 **`sizeof(Slot)`** 实测为准）。

| 步骤 | 源码表达式 | 代入数字后的结果 |
|------|------------|------------------|
| 块头宽度 | `(char*)newBlock + sizeof(Slot*)` | 首个 **`body`** 相对 **`base`**：**`+8`** 字节 |
| 对齐填充 | `padPointer(body, SlotSize_)` | **`rem = 8 % 16 = 8`** → **`paddingSize = 16 - 8 = 8`** |
| 首个槽起点 | `body + paddingSize` | 相对 **`base`**：**`8 + 8 = 16`**（已是 **`SlotSize_`** 倍数）→ **`curSlot_` 初次指向此处** |
| 上界哨兵 | `(size_t)newBlock + BlockSize_ - SlotSize_ + 1` | 相对 **`base`**：**`4096 - 16 + 1 = 4081`** → **`lastSlot_`**（与 **`curSlot_`** 比较用；**不必对齐到 `SlotSize_`**，属当前实现写法） |
| 指针步进 | `curSlot_ += SlotSize_ / sizeof(Slot)` | **`16 / 8 = 2`**（每次 **`Slot*`** 前进 2 格 × **`sizeof(Slot)=8`** ⇒ **16 字节**，与 **`SlotSize_`** 一致） |

**偏移手绘（相对 `base`，单位：字节）**：

```
  0        8        16                              4081              4096
  ├────────┼────────┼───────────────────────────────┼─────────────────┤
  │块头 8B │padding │ 槽0 │ 槽1 │ …（顺序分配游标在此推进）…           │
  │next链  │  8B    │◄── curSlot_ 首次在此           │◄── lastSlot_     │
```

**槽个数量级（直观，非替换源码判定逻辑）**：槽区约占 **`4096 - 16 = 4080`** 字节量级（减去 **`base→body` 的 8B** 与 **`padding` 8B** 后用于槽的有效区间略小）；**`4080 / 16 ≈ 255`**，与 **`lastSlot_`** 的比较共同决定何时 **`allocateNewBlock()`**。

---

### 7.3 多块 Block：`firstBlock_` 链起来的含义（配合析构）

**写入**：每次 `allocateNewBlock()` **头插**——新块的「块头」里 **`next`** 指向原来的 **`firstBlock_`**，然后 **`firstBlock_ = newBlock`**。

**手绘链表方向**（从 **`firstBlock_`** 出发，`next` 沿「更早分配的块」前进）：

```
        firstBlock_
             │
             ▼
       ┌───────────┐      ┌───────────┐      ┌───────────┐
       │ Block 新   │ next │ Block 旧   │ next │ Block 更旧 │ next = nullptr
       │ (最近一次   │ ──► │           │ ──► │           │
       │ allocateNew)│      │           │      │           │
       └───────────┘      └───────────┘      └───────────┘
```

**遍历释放**：`MemoryPool::~MemoryPool()`（约 39–58 行）  
`cur = firstBlock_` → 循环：`next = cur->next` → `operator delete(cur)` → `cur = next`。

---

### 7.4 顺序分配：`allocate()` 里 `curSlot_` 与 `lastSlot_`

**函数**：`void* MemoryPool::allocate()`（约 94–117 行）。

1. **`popFreeList()`** 得到栈顶 **`Slot*`** → 直接 **`return`**（指向 **先前 `deallocate` 归还**、仍在某 Block 槽区内的地址）。
2. 否则进 **`mutexForBlock_`**：
   - 若 **`curSlot_ >= lastSlot_`** → **`allocateNewBlock()`**；
   - **`temp = curSlot_`**；
   - **`curSlot_ += SlotSize_ / sizeof(Slot)`** —— **`Slot*`** 指针算术：每次前进 **`(SlotSize_/sizeof(Slot)) * sizeof(Slot)`** 字节（通常 **`sizeof(Slot)`** 为 8，`SlotSize_` 为 8 的倍数，故步长约等于 **`SlotSize_`**）。

```
       同一 Block 槽区内（示意）

       ┌────────┬────────┬────────┬─────┬────────┐
       │ 槽 0    │ 槽 1    │ 槽 2    │ ... │ 不可再切 │
       └────────┴────────┴────────┴─────┴────────┘
         ▲                           ▲
         │                           │
      某次 temp=curSlot_          lastSlot_（上界判断用）
      然后 curSlot_ 前进一格
```

---

### 7.5 空闲栈 `freeList_`：与 Block 物理位置的关系

**函数**：`pushFreeList(Slot* slot)`、`popFreeList()`。

- **逻辑结构**：Treiber 无锁栈；**栈结点**就是 **`allocate` 曾返回给用户、经 `deallocate` 归还的槽起始地址**。
- **物理上**：这些 **`Slot*` 仍指向 **各 Block 槽区内部**某格，**不一定是连续链表在堆上 malloc 出来的**；只是用 **`slot->next` 原子** 串成栈。

```
  freeList_（atomic 头）──► [槽A 在 Block1] ──► [槽B 在 Block2] ──► nullptr
                              next 原子           next 原子
```

---

### 7.6 全局路由：`HashBucket` 与 `memoryPool[]`

**函数**：`HashBucket::initMemoryPool()`、`HashBucket::getMemoryPool(int index)`（`MemoryPool.cpp` 后部）。

```
  getMemoryPool(index) 内部：
  ┌─────────────────────────────────────────────────────────┐
  │ static MemoryPool memoryPool[MEMORY_POOL_NUM];  // 64 个 │
  └─────────────────────────────────────────────────────────┘
         memoryPool[0]  SlotSize_=8
         memoryPool[1]  SlotSize_=16
         ...
         memoryPool[63] SlotSize_=512

  useMemory(size)  （内联于 MemoryPool.h）
       │
       ├─ size > MAX_SLOT_SIZE ──► operator new(size)
       └─ 否则 index = ((size+7)/8)-1 ──► memoryPool[index].allocate()
```

---

### 7.7 从 `newElement` 到槽：指针类型与职责分界

**函数**：`newElement`（`MemoryPool.h` 模板）、内部调用 **`HashBucket::useMemory(sizeof(T))`**。

```
  newElement<T>(...)
        │
        ▼
  useMemory(sizeof(T))  ──►  某档 MemoryPool::allocate()  ──►  void* / Slot* 起始
        │
        ▼
  reinterpret_cast<T*>(...)  ──►  new(p) T(...)   placement new 构造对象
```

**`deleteElement`**：逆过程 —— **`p->~T()`** → **`freeMemory(p, sizeof(T))`** → 与分配时同一 **index** 公式进池。

---

## 8. 必备知识点 / API 速查

### 8.1 C++ 语言与标准库

| 概念 | 在本项目中的体现 |
|------|------------------|
| 声明与定义分离 | `.h` 声明，`.cpp` 定义（避免 ODR 多重定义） |
| `void*` | 池对外「未类型化」指针 |
| `reinterpret_cast` | 在程序员保证布局正确的前提下做指针reinterpret |
| placement new | `new (p) T(...)` |
| 显式析构 | `p->~T()` |
| `static` 成员函数 | `HashBucket::...` |
| `std::mutex` + `std::lock_guard` | 扩块 / 顺序切槽互斥 |
| `std::atomic` + `compare_exchange_weak` | 无锁栈 |
| `std::memory_order_*` | 原子操作内存序（与并发正确性相关，深入需专门学） |
| 模板 | `newElement` / `deleteElement` |
| 完美转发 | `Args&&...` + `std::forward` |

### 8.2 对外推荐 API（常规用法）

```cpp
using namespace Kama_memoryPool;

HashBucket::initMemoryPool();

auto* p = newElement<MyType>(构造参数...);
if (p) {
    // 使用 *p
    deleteElement<MyType>(p);
}
```

底层用法（自担类型与安全责任）：

```cpp
void* raw = HashBucket::useMemory(n);
// ... 手动 placement new / 手动 ~T() ...
HashBucket::freeMemory(raw, n);   // n 必须与 useMemory(n) 一致
```

---

## 9. 工程与实践注意事项

1. **链接**：单元测试或你自己的 `main` 必须 **`#include MemoryPool.h`** 且链接 **`MemoryPool.cpp`**（或通过 CMake 同一 target）。  
2. **初始化顺序**：任何 **`useMemory` / `newElement`** 之前调用 **`initMemoryPool()`**。  
3. **配对**：`useMemory(n)` ↔ `freeMemory(ptr, n)`；`newElement<T>` ↔ `deleteElement<T>`，不要混用不一致的 size。  
4. **benchmark**：`UnitTest.cpp` 使用 **`clock()`**，粒度粗、负载小时易出现 **0 ms**；严肃对比应用高精度时钟并增大迭代次数。  
5. **健壮性**：教学 / Demo 向代码；工业级还需考虑对齐、`OOM`、调试断言、`atomic` 与块链表遍历等细节的严格规范化。

---

## 10. 建议的阅读顺序

1. 本 **`README.md`** 前几节（全局图）  
2. 本 **`README.md` 第 7 节**（内存布局与源码变量、函数对照图）  
3. **`include/MemoryPool.h`**（常量、`HashBucket::useMemory/freeMemory`、`newElement/deleteElement`）  
4. **`src/MemoryPool.cpp`**（`allocate`、`allocateNewBlock`、`push/popFreeList`）  
5. **`tests/UnitTest.cpp`**（怎么用、如何对比）

---

## 11. 常见问题（FAQ）

**Q：为什么 Code Runner 只跑 `UnitTest.cpp` 会链接报错？**  
A：实现不在测试文件里，需要把 **`MemoryPool.cpp`** 一起编译链接。

**Q：`placement new` 在哪？**  
A：在 **`MemoryPool.h`** 的 **`newElement`** 模板内：`new(p) T(...)`。

**Q：`HashBucket` 和「哈希表」有关系吗？**  
A：名字类比「按 key（尺寸）分桶」；实现上是数组下标路由，不一定是哈希函数。

---

**图示说明**：本章第 7 节为与 `MemoryPool.h` / `MemoryPool.cpp` 变量、函数对齐的内存布局手绘；若你修改了块头跳过字节数或 `lastSlot_` 公式，请同步更新图示与行号引用。
