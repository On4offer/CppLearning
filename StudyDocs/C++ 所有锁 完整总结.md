# 完整版：C++ 全量锁体系
覆盖：**标准库锁 + 底层内核锁 + 分配器Arena/TCache锁 + 自旋锁/混合锁 + 屏障/序列锁**，补齐你之前提到的 `ptmalloc/main_arena/tcache` 整套内存分配锁，全覆盖无遗漏。

## 一、C++ 标准库 互斥原语（C++11~C++23 完整）
### 1. 基础独占锁
1. `std::mutex`
   非递归、独占、阻塞，底层 Linux 依赖 `futex`。
2. `std::recursive_mutex`
   同一线程可多次加锁，计数释放，开销高于普通mutex。
3. `std::timed_mutex`
   带超时阻塞，`try_lock_for / try_lock_until`。
4. `std::recursive_timed_mutex`
   可重入 + 超时。

### 2. 读写锁
5. `std::shared_mutex`（C++17）
   共享读、独占写；读多写少最优。
6. `std::shared_timed_mutex`（C++14）
   读写锁 + 超时能力。

### 3. RAII 锁托管（不是锁，是锁管理器）
- `std::lock_guard`：极简RAII，不可手动解锁
- `std::unique_lock`：可延迟加锁、手动解锁、配合条件变量
- `std::shared_lock`：专门托管共享读锁

### 4. 轻量同步原语（广义锁范畴）
- `std::atomic<T>`：**无锁原子指令**，CAS/LL/SC，纯用户态
- `std::semaphore`（C++20）：计数信号量，限流、生产者消费者
- `std::latch / std::barrier`（C++20）：线程批量等待屏障
- `std::once_flag`：单次执行同步

---

## 二、OS 原生底层锁（Linux pthread / Windows 内核）
### Linux POSIX 系列
1. **pthread_mutex_t**
   标准线程互斥锁，`std::mutex` 底层实现。
2. **pthread_rwlock_t**
   原生读写锁。
3. **pthread_spinlock_t**
   原生**自旋锁**，用户态空转，无内核休眠。
4. **futex**
   Linux 底层同步基石；
   竞争轻微：用户态原子操作；
   竞争激烈：陷入内核休眠，**混合锁实现**。

### Windows 原生锁
1. `CRITICAL_SECTION`：用户态临界区，轻量
2. `SRWLock`：读写锁
3. `Event/Mutex/Semaphore`：内核态重量级同步体

---

## 三、内存分配器专属锁（你之前问的核心缺失部分）
### ptmalloc(glibc malloc) 全套锁
1. **main_arena 全局堆锁**
   早期glibc唯一大锁，所有线程争抢，高并发严重瓶颈。
2. **per-thread arena 分区锁**
   多Arena分片，每个Arena独立mutex，分散锁竞争。
3. **tcache 线程本地缓存锁**
   glibc2.26+，**每个线程独立无锁缓存**；
   小对象优先走tcache，完全避开arena锁，大幅优化并发。
4. **brk/mmap 系统调用内核锁**
   堆扩容/匿名内存映射时，触发内核内存管理锁。

### 其他分配器锁
- jemalloc：多级分区+线程本地缓存+细粒度分片锁
- tcmalloc：Per-CPU 缓存、细粒度锁，极致降低竞争

---

## 四、高性能自定义锁（工程/框架常用，标准库无）
### 1. 自旋锁 SpinLock
- 原理：死循环CAS空转，不主动放弃CPU
- 适用：**临界区极短**（几十条指令）
- 缺点：临界区长会空转浪费CPU

### 2. 混合锁（自适应锁）
- 轻竞争：自旋等待
- 高竞争：主动休眠阻塞
- `futex`、主流mutex 都是混合锁

### 3. 排他自旋读/写锁
用户态自研RW自旋锁，用于高吞吐中间件。

### 4. 序列锁 Seqlock
- 无锁读写，不阻塞
- 适合：高频读、极少写、简单数值
- 常见于内核、监控指标采集

### 5. 偏向锁 / 轻量级锁
JVM概念，但C++高并发组件也有同类设计：
单线程无竞争时完全无锁，多线程退化普通锁。

---

## 五、特殊同步机制（广义锁）
1. **条件变量 `std::condition_variable`**
   必须搭配`unique_lock`，用于线程等待-唤醒。
2. **屏障 Barrier**
   一批线程全部到达临界点再统一放行。
3. **顺序锁、RCU**
   无锁延迟回收，大数据、内核、高并发服务端常用。

---

## 六、关键：锁性能梯度（完整排序）
从快 → 慢
1. 无锁：`std::atomic`、RCU、Seqlock
2. 用户态自旋锁：`pthread_spinlock_t`、自定义SpinLock
3. tcache无锁分配
4. 共享读锁：`std::shared_mutex` 读
5. 混合锁：`std::mutex`（futex）
6. 分区锁：arena 独立mutex
7. 可重入锁：`recursive_mutex`
8. 读写写锁：`shared_mutex` 写
9. 全局大锁：main_arena 全局堆锁
10. 内核态重量级锁：mmap/brk 内核锁、Windows 内核Mutex

---

## 七、面试精简背诵版（全覆盖）
1. 标准库：`mutex/recursive_mutex/timed_mutex/shared_mutex` + RAII 三件套
2. 底层OS：`futex`、pthread 系列互斥/读写/自旋锁
3. 内存分配器：main_arena全局锁、per-arena分区锁、tcache线程本地无锁缓存、内核内存锁
4. 高性能自研：自旋锁、自适应混合锁、seqlock、RCU
5. 广义同步：原子、信号量、屏障、条件变量

需要我给你整理一份**「每种锁的底层原理+适用场景+缺陷」**极简对照表，直接背诵面试吗？