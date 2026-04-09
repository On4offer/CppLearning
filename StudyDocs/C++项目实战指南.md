# C++ 项目实战指南

本文档提供 C++ 项目开发的实战指南，涵盖构建系统、调试技巧、代码规范等核心内容，帮助您从代码编写过渡到完整项目开发。

## 一、构建系统

### 1. CMake 基础

#### 1.1 基本概念
- **CMake**：跨平台构建系统生成器，用于生成各平台的构建文件
- **CMakeLists.txt**：CMake 配置文件，定义项目结构和构建规则
- **构建目录**：建议使用单独的构建目录，避免污染源码

#### 1.2 基本 CMakeLists.txt 结构

```cmake
# 最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(MyProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(MyApp main.cpp)

# 添加库
add_library(MyLib STATIC src/lib.cpp)

# 链接库
target_link_libraries(MyApp MyLib)

# 包含头文件目录
target_include_directories(MyApp PRIVATE include)
```

#### 1.3 常用 CMake 命令

| 命令 | 作用 | 示例 |
|------|------|------|
| `add_executable` | 添加可执行文件 | `add_executable(App main.cpp)` |
| `add_library` | 添加库 | `add_library(Lib STATIC lib.cpp)` |
| `target_link_libraries` | 链接库 | `target_link_libraries(App Lib)` |
| `target_include_directories` | 包含头文件目录 | `target_include_directories(App PRIVATE include)` |
| `set` | 设置变量 | `set(CMAKE_CXX_STANDARD 11)` |
| `find_package` | 查找包 | `find_package(Boost REQUIRED)` |

#### 1.4 构建步骤

```bash
# 创建构建目录
mkdir build
cd build

# 生成构建文件
cmake ..

# 编译
cmake --build .

# 运行
./MyApp
```

### 2. 其他构建系统

- **Makefile**：传统构建系统，适用于简单项目
- **QMake**：Qt 项目的构建系统
- **Meson**：现代化构建系统，速度快
- **Bazel**：Google 开发的构建系统，适用于大型项目

## 二、调试技巧

### 1. GDB 调试

#### 1.1 基本命令

| 命令 | 作用 | 示例 |
|------|------|------|
| `gdb ./app` | 启动 GDB 调试 | `gdb ./MyApp` |
| `run` | 运行程序 | `run` |
| `break` | 设置断点 | `break main.cpp:42` |
| `continue` | 继续执行 | `continue` |
| `step` | 单步进入 | `step` |
| `next` | 单步跳过 | `next` |
| `print` | 打印变量值 | `print x` |
| `backtrace` | 查看调用栈 | `backtrace` |
| `quit` | 退出 GDB | `quit` |

#### 1.2 常用调试场景

```bash
# 启动带调试信息的编译
g++ -g main.cpp -o app

# 运行 GDB
gdb ./app

# 设置断点
b break main.cpp:20

# 运行程序
run

# 打印变量
p variable

# 查看内存
x/10x &variable

# 查看调用栈
bt
```

### 2. VS Code 调试

#### 2.1 配置 launch.json

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C++ Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/MyApp",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build"
        }
    ]
}
```

#### 2.2 配置 tasks.json

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/build"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

### 3. 日志调试

#### 3.1 自定义日志宏

```cpp
#include <iostream>
#include <string>

#define LOG_INFO(msg) std::cout << "[INFO] " << msg << std::endl
#define LOG_WARN(msg) std::cout << "[WARN] " << msg << std::endl
#define LOG_ERROR(msg) std::cerr << "[ERROR] " << msg << std::endl

// 使用
LOG_INFO("程序启动");
LOG_WARN("可能的问题");
LOG_ERROR("严重错误");
```

#### 3.2 条件断点

- **GDB**：`break main.cpp:42 if x > 10`
- **VS Code**：在断点设置中添加条件

## 三、代码规范

### 1. Google C++ Style Guide 要点

#### 1.1 命名约定

- **文件命名**：小写字母，下划线分隔，如 `my_file.cpp`
- **类名**：大驼峰，如 `MyClass`
- **函数名**：小驼峰，如 `myFunction()`
- **变量名**：小写字母，下划线分隔，如 `my_variable`
- **常量名**：全大写，下划线分隔，如 `MAX_VALUE`
- **成员变量**：小写字母，下划线结尾，如 `my_member_`

#### 1.2 代码风格

- **缩进**：4 个空格（不使用制表符）
- **行宽**：不超过 80 列
- **括号**：K&R 风格（左括号在同一行）
- **空格**：操作符前后加空格，逗号后加空格

```cpp
// 正确
if (condition) {
    // 代码
}

// 错误
if(condition){
// 代码
}
```

#### 1.3 注释

- **文件头**：包含版权、作者、功能描述
- **函数注释**：描述功能、参数、返回值
- **复杂代码**：添加解释性注释

```cpp
// 计算两个数的和
// @param a 第一个数
// @param b 第二个数
// @return 和
int add(int a, int b) {
    return a + b;
}
```

### 2. 代码审查

- **自我审查**：编译前检查代码逻辑
- **同伴审查**：互相检查代码质量
- **工具审查**：使用静态分析工具

## 四、版本控制

### 1. Git 基础

#### 1.1 常用命令

| 命令 | 作用 | 示例 |
|------|------|------|
| `git init` | 初始化仓库 | `git init` |
| `git clone` | 克隆仓库 | `git clone https://github.com/user/repo.git` |
| `git add` | 添加文件 | `git add .` |
| `git commit` | 提交更改 | `git commit -m "Add feature"` |
| `git push` | 推送更改 | `git push origin main` |
| `git pull` | 拉取更改 | `git pull origin main` |
| `git branch` | 查看分支 | `git branch` |
| `git checkout` | 切换分支 | `git checkout feature` |
| `git merge` | 合并分支 | `git merge feature` |

#### 1.2 分支管理

- **main/master**：主分支，保持稳定
- **develop**：开发分支，集成新功能
- **feature**：特性分支，开发新功能
- **bugfix**：修复分支，修复 bug

#### 1.3 提交规范

- **提交信息**：简洁明了，描述更改内容
- **格式**：`类型: 描述`，如 `feat: Add login feature`
- **类型**：
  - `feat`：新功能
  - `fix`：修复 bug
  - `docs`：文档更改
  - `style`：代码风格
  - `refactor`：重构
  - `test`：测试
  - `chore`：构建/依赖更新

## 五、依赖管理

### 1. 第三方库引入

#### 1.1 CMake 包管理

```cmake
# 查找包
find_package(Boost REQUIRED COMPONENTS filesystem)

# 链接库
target_link_libraries(MyApp Boost::filesystem)

# 包含头文件
target_include_directories(MyApp PRIVATE ${Boost_INCLUDE_DIRS})
```

#### 1.2 子模块管理

```bash
# 添加子模块
git submodule add https://github.com/libgit2/libgit2.git extern/libgit2

# 更新子模块
git submodule update --init --recursive
```

#### 1.3 包管理器

- **vcpkg**：Microsoft 开发的 C++ 包管理器
- **conan**：跨平台包管理器
- **C++ Package Manager (CPM)**：基于 CMake 的包管理器

## 六、跨平台开发

### 1. 平台差异

| 平台 | 差异点 | 解决方案 |
|------|--------|----------|
| Windows | 路径分隔符 `\` | 使用 `std::filesystem` |
| Linux | 路径分隔符 `/` | 使用 `std::filesystem` |
| Windows | 系统调用 | 使用 `Windows API` 或跨平台库 |
| Linux | 系统调用 | 使用 `POSIX API` 或跨平台库 |

### 2. 跨平台库

- **Boost**：提供大量跨平台工具
- **Qt**：跨平台 GUI 库
- **SDL**：跨平台游戏开发库
- **Curl**：跨平台网络库
- **zlib**：跨平台压缩库

### 3. 条件编译

```cpp
#ifdef _WIN32
    // Windows 特定代码
    #include <windows.h>
#elif __linux__
    // Linux 特定代码
    #include <unistd.h>
#endif
```

## 七、项目结构

### 1. 典型项目结构

```
MyProject/
├── CMakeLists.txt      # 主 CMake 配置
├── include/            # 头文件
│   └── mylib/          # 库头文件
├── src/                # 源代码
│   ├── main.cpp        # 主函数
│   └── mylib/          # 库源代码
├── test/               # 测试代码
│   └── test_mylib.cpp  # 库测试
├── extern/             # 第三方库
├── doc/                # 文档
└── build/              # 构建目录
```

### 2. 目录组织原则

- **分离接口和实现**：头文件放在 `include`，实现放在 `src`
- **模块化**：按功能组织代码
- **测试覆盖**：为每个模块编写测试
- **文档同步**：保持文档与代码一致

## 八、实战案例

### 1. 命令行工具开发

#### 1.1 项目结构

```
cli_tool/
├── CMakeLists.txt
├── include/
│   └── cli/
│       ├── parser.h
│       └── commands.h
├── src/
│   ├── main.cpp
│   └── cli/
│       ├── parser.cpp
│       └── commands.cpp
└── test/
    └── test_parser.cpp
```

#### 1.2 核心功能

- **命令行参数解析**：使用 `getopt` 或第三方库
- **命令执行**：根据参数执行对应功能
- **错误处理**：友好的错误提示

### 2. 简单网络服务器

#### 2.1 项目结构

```
web_server/
├── CMakeLists.txt
├── include/
│   └── server/
│       ├── http.h
│       └── server.h
├── src/
│   ├── main.cpp
│   └── server/
│       ├── http.cpp
│       └── server.cpp
└── test/
    └── test_http.cpp
```

#### 2.2 核心功能

- **HTTP 协议解析**：处理请求和响应
- **socket 编程**：处理网络连接
- **线程池**：并发处理请求

## 九、总结

C++ 项目开发需要综合考虑多个方面，包括构建系统、调试技巧、代码规范、版本控制等。通过本文档的学习，您应该能够：

1. **搭建项目框架**：使用 CMake 构建跨平台项目
2. **调试问题**：使用 GDB 或 IDE 调试工具定位和解决问题
3. **编写规范代码**：遵循代码规范，提高代码质量
4. **管理项目**：使用 Git 进行版本控制，管理依赖
5. **跨平台开发**：处理不同平台的差异

这些技能将帮助您从一个 C++ 初学者成长为能够独立开发完整项目的专业开发者。在实际项目中，您还需要不断学习和积累经验，逐步提高自己的开发能力。