# C++ STL 容器学习 Demo

本目录用**可编译运行**的小程序演示常用 STL 容器的典型用法，配合注释理解「何时用、怎么查、常见坑」。

**编译（与 Algorithms 目录一致）：**

```bash
g++ -std=c++17 -O2 某文件.cpp -o run.exe
```

`11_optional_variant.cpp` 等依赖 **C++17**，请确保加上 `-std=c++17`。

## 文件一览

| 文件 | 内容概要 |
|------|----------|
| `01_vector_array.cpp` | `vector`、`array`：随机访问、扩容、`emplace` |
| `02_deque_list.cpp` | `deque`、`list`、`forward_list`：头尾插入、迭代器特点 |
| `03_set_map.cpp` | `set`/`multiset`、`map`/`multimap`：有序、去重、下标与 `find` |
| `04_unordered.cpp` | `unordered_set`/`unordered_map`：哈希、自定义哈希简述 |
| `05_stack_queue.cpp` | `stack`、`queue`、`priority_queue`：单调栈/单调队列入口 |
| `06_string_utility.cpp` | `string`、`pair`、`tuple`、`bitset` 常用片段 |
| `07_algorithm.cpp` | `<algorithm>`：`sort`、二分、`unique`、`next_permutation`、`partition` 等 |
| `08_iterator.cpp` | 迭代器：`reverse_iterator`、`advance`/`next`/`prev`、失效提示 |
| `09_numeric.cpp` | `<numeric>`：`iota`、`accumulate`、`partial_sum`、`gcd`/`lcm` |
| `10_memory_smart_ptr.cpp` | `<memory>`：`unique_ptr`、`shared_ptr`、`weak_ptr` |
| `11_optional_variant.cpp` | C++17：`optional`、`variant`、`string_view` |

建议顺序：**01 → 11**；容器（01–06）与算法（07–09）可交叉看，**07** 与算法题关系最紧。
