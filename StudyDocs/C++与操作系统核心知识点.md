# C++与操作系统核心知识点

本文档涵盖操作系统的核心知识点，包括进程与线程管理、内存管理、文件系统、系统调用等内容，结合C++代码示例，帮助您在校招笔试面试中掌握操作系统相关知识。

## 一、进程与线程

### 1. 进程管理

#### 1.1 进程创建

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        // 创建进程失败
        cerr << "Fork failed" << endl;
        return 1;
    } else if (pid == 0) {
        // 子进程
        cout << "Child process, PID: " << getpid() << endl;
        cout << "Parent PID: " << getppid() << endl;
        // 执行新程序
        execl("/bin/ls", "ls", "-l", nullptr);
        // 如果execl成功，下面的代码不会执行
        cerr << "execl failed" << endl;
        return 1;
    } else {
        // 父进程
        cout << "Parent process, PID: " << getpid() << endl;
        cout << "Child PID: " << pid << endl;
        // 等待子进程结束
        int status;
        wait(&status);
        cout << "Child process exited with status: " << WEXITSTATUS(status) << endl;
    }
    
    return 0;
}
```

#### 1.2 进程通信

##### 1.2.1 管道通信

```cpp
#include <iostream>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int pipefd[2];
    char buffer[1024];
    
    // 创建管道
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed" << endl;
        return 1;
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    } else if (pid == 0) {
        // 子进程：读端
        close(pipefd[1]); // 关闭写端
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            cout << "Child received: " << buffer << endl;
        }
        close(pipefd[0]);
    } else {
        // 父进程：写端
        close(pipefd[0]); // 关闭读端
        const char* message = "Hello from parent!";
        write(pipefd[1], message, strlen(message));
        close(pipefd[1]);
        // 等待子进程结束
        wait(nullptr);
    }
    
    return 0;
}
```

##### 1.2.2 共享内存

```cpp
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>
using namespace std;

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    
    if (shmid == -1) {
        cerr << "shmget failed" << endl;
        return 1;
    }
    
    // 连接到共享内存
    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    
    pid_t pid = fork();
    
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    } else if (pid == 0) {
        // 子进程：读取共享内存
        cout << "Child reading: " << shmaddr << endl;
        // 修改共享内存
        strcpy(shmaddr, "Modified by child");
    } else {
        // 父进程：写入共享内存
        strcpy(shmaddr, "Hello from parent");
        // 等待子进程结束
        wait(nullptr);
        cout << "Parent reading: " << shmaddr << endl;
        // 分离共享内存
        shmdt(shmaddr);
        // 删除共享内存
        shmctl(shmid, IPC_RMID, nullptr);
    }
    
    return 0;
}
```

### 2. 线程管理

#### 2.1 线程创建与同步

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

mutex mtx;
int counter = 0;

void increment(int id, int iterations) {
    for (int i = 0; i < iterations; i++) {
        lock_guard<mutex> lock(mtx);
        counter++;
        cout << "Thread " << id << ": counter = " << counter << endl;
    }
}

int main() {
    vector<thread> threads;
    int num_threads = 5;
    int iterations = 10;
    
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(increment, i, iterations);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "Final counter value: " << counter << endl;
    return 0;
}
```

#### 2.2 条件变量

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

mutex mtx;
condition_variable cv;
queue<int> tasks;
bool done = false;

void producer() {
    for (int i = 0; i < 10; i++) {
        {   
            lock_guard<mutex> lock(mtx);
            tasks.push(i);
            cout << "Produced: " << i << endl;
        }
        cv.notify_one();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    {   
        lock_guard<mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return !tasks.empty() || done; });
        
        if (done && tasks.empty()) {
            break;
        }
        
        int task = tasks.front();
        tasks.pop();
        cout << "Consumed: " << task << endl;
    }
}

int main() {
    thread producer_thread(producer);
    thread consumer_thread(consumer);
    
    producer_thread.join();
    consumer_thread.join();
    
    return 0;
}
```

#### 2.3 线程池

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
using namespace std;

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop;
public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; i++) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(this->mtx);
                        this->cv.wait(lock, [this] { 
                            return this->stop || !this->tasks.empty(); 
                        });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        task = move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    
    template<class F>
    void enqueue(F&& f) {
        {
            unique_lock<mutex> lock(mtx);
            tasks.emplace(forward<F>(f));
        }
        cv.notify_one();
    }
    
    ~ThreadPool() {
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (thread &worker : workers) {
            worker.join();
        }
    }
};

// 测试代码
int main() {
    ThreadPool pool(4);
    
    for (int i = 0; i < 8; i++) {
        pool.enqueue([i] {
            cout << "Task " << i << " executed by thread " << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::seconds(1));
        });
    }
    
    // 等待所有任务完成
    this_thread::sleep_for(chrono::seconds(3));
    
    return 0;
}
```

## 二、内存管理

### 1. 内存分配

#### 1.1 堆内存分配

```cpp
#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    // 使用malloc分配内存
    int* arr1 = (int*)malloc(5 * sizeof(int));
    if (arr1 == nullptr) {
        cerr << "malloc failed" << endl;
        return 1;
    }
    
    // 使用calloc分配并初始化内存
    int* arr2 = (int*)calloc(5, sizeof(int));
    if (arr2 == nullptr) {
        cerr << "calloc failed" << endl;
        free(arr1);
        return 1;
    }
    
    // 使用realloc调整内存大小
    int* arr3 = (int*)realloc(arr1, 10 * sizeof(int));
    if (arr3 == nullptr) {
        cerr << "realloc failed" << endl;
        free(arr1);
        free(arr2);
        return 1;
    }
    
    // 赋值
    for (int i = 0; i < 10; i++) {
        arr3[i] = i;
    }
    
    // 打印
    cout << "arr3: ";
    for (int i = 0; i < 10; i++) {
        cout << arr3[i] << " ";
    }
    cout << endl;
    
    cout << "arr2: ";
    for (int i = 0; i < 5; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;
    
    // 释放内存
    free(arr3);
    free(arr2);
    
    return 0;
}
```

#### 1.2 C++内存分配

```cpp
#include <iostream>
using namespace std;

int main() {
    // 分配单个对象
    int* p1 = new int(10);
    cout << "*p1 = " << *p1 << endl;
    
    // 分配数组
    int* arr = new int[5];
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    
    cout << "arr: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // 释放内存
    delete p1;
    delete[] arr;
    
    return 0;
}
```

### 2. 内存映射

```cpp
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    const char* filename = "mmap_test.txt";
    const char* message = "Hello, memory mapping!";
    
    // 打开文件
    int fd = open(filename, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        cerr << "open failed" << endl;
        return 1;
    }
    
    // 设置文件大小
    off_t size = strlen(message) + 1;
    if (ftruncate(fd, size) == -1) {
        cerr << "ftruncate failed" << endl;
        close(fd);
        return 1;
    }
    
    // 内存映射
    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        cerr << "mmap failed" << endl;
        close(fd);
        return 1;
    }
    
    // 写入数据
    strcpy((char*)addr, message);
    
    // 同步到文件
    if (msync(addr, size, MS_SYNC) == -1) {
        cerr << "msync failed" << endl;
    }
    
    // 读取数据
    cout << "Read from mmap: " << (char*)addr << endl;
    
    // 解除映射
    if (munmap(addr, size) == -1) {
        cerr << "munmap failed" << endl;
    }
    
    close(fd);
    return 0;
}
```

### 3. 内存泄漏检测

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Resource {
public:
    Resource() {
        cout << "Resource acquired" << endl;
    }
    ~Resource() {
        cout << "Resource released" << endl;
    }
};

void leakyFunction() {
    // 内存泄漏：未释放内存
    Resource* r = new Resource();
    // 应该使用：delete r;
}

void safeFunction() {
    // 使用智能指针，自动释放内存
    unique_ptr<Resource> r(new Resource());
}

int main() {
    cout << "Calling leakyFunction()" << endl;
    leakyFunction();
    cout << "leakyFunction() returned" << endl;
    
    cout << "\nCalling safeFunction()" << endl;
    safeFunction();
    cout << "safeFunction() returned" << endl;
    
    return 0;
}
```

## 三、文件系统

### 1. 文件操作

#### 1.1 基本文件操作

```cpp
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // 写入文件
    ofstream outFile("test.txt");
    if (!outFile) {
        cerr << "Failed to open file for writing" << endl;
        return 1;
    }
    
    outFile << "Hello, file system!" << endl;
    outFile << "This is a test." << endl;
    outFile.close();
    
    // 读取文件
    ifstream inFile("test.txt");
    if (!inFile) {
        cerr << "Failed to open file for reading" << endl;
        return 1;
    }
    
    string line;
    cout << "File content:" << endl;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
    
    // 追加内容
    ofstream appendFile("test.txt", ios::app);
    if (!appendFile) {
        cerr << "Failed to open file for appending" << endl;
        return 1;
    }
    
    appendFile << "Appended line." << endl;
    appendFile.close();
    
    // 再次读取
    inFile.open("test.txt");
    cout << "\nFile content after appending:" << endl;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
    
    return 0;
}
```

#### 1.2 文件系统操作

```cpp
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

int main() {
    // 当前目录
    cout << "Current directory: " << fs::current_path() << endl;
    
    // 创建目录
    fs::create_directory("test_dir");
    cout << "Created directory: test_dir" << endl;
    
    // 遍历目录
    cout << "\nDirectory contents:" << endl;
    for (const auto& entry : fs::directory_iterator(".")) {
        cout << entry.path() << " (" << (fs::is_directory(entry) ? "directory" : "file") << ")" << endl;
    }
    
    // 重命名文件/目录
    fs::rename("test_dir", "renamed_dir");
    cout << "\nRenamed directory to: renamed_dir" << endl;
    
    // 删除目录
    fs::remove_all("renamed_dir");
    cout << "Removed directory: renamed_dir" << endl;
    
    return 0;
}
```

### 2. I/O 多路复用

#### 2.1 select

```cpp
#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // 创建 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 设置 socket 选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    // 绑定 socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    fd_set readfds;
    int max_fd = server_fd;
    
    while (true) {
        // 清空集合
        FD_ZERO(&readfds);
        
        // 添加服务器 socket
        FD_SET(server_fd, &readfds);
        
        // 等待活动
        cout << "Waiting for connections..." << endl;
        int activity = select(max_fd + 1, &readfds, nullptr, nullptr, nullptr);
        
        if (activity < 0 && errno != EINTR) {
            perror("select error");
        }
        
        // 检查服务器 socket
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            
            cout << "New connection, socket fd is " << new_socket << endl;
            
            // 发送欢迎消息
            const char* hello = "Hello from server";
            send(new_socket, hello, strlen(hello), 0);
            cout << "Welcome message sent" << endl;
            
            // 更新最大 fd
            if (new_socket > max_fd) {
                max_fd = new_socket;
            }
        }
        
        // 这里可以添加对其他客户端 socket 的处理
    }
    
    return 0;
}
```

#### 2.2 epoll

```cpp
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
using namespace std;

#define MAX_EVENTS 10

int main() {
    int server_fd, new_socket, epoll_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // 创建 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 设置 socket 选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    // 绑定
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // 创建 epoll 实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    
    // 添加服务器 socket 到 epoll
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }
    
    struct epoll_event events[MAX_EVENTS];
    
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 新连接
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                
                cout << "New connection, socket fd is " << new_socket << endl;
                
                // 添加新 socket 到 epoll
                event.events = EPOLLIN | EPOLLET; // 边缘触发
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }
            } else {
                // 现有连接的数据
                int fd = events[i].data.fd;
                int valread = read(fd, buffer, 1024);
                if (valread == 0) {
                    // 连接关闭
                    cout << "Connection closed, fd: " << fd << endl;
                    close(fd);
                } else {
                    // 处理数据
                    buffer[valread] = '\0';
                    cout << "Received: " << buffer << endl;
                    send(fd, buffer, valread, 0);
                }
            }
        }
    }
    
    return 0;
}
```

## 四、系统调用

### 1. 进程控制

| 系统调用 | 功能 | 示例 |
|---------|------|------|
| `fork()` | 创建子进程 | `pid_t pid = fork();` |
| `exec()` | 执行新程序 | `execl("/bin/ls", "ls", "-l", NULL);` |
| `wait()` | 等待子进程结束 | `wait(&status);` |
| `exit()` | 终止进程 | `exit(0);` |
| `getpid()` | 获取进程ID | `pid_t pid = getpid();` |
| `getppid()` | 获取父进程ID | `pid_t ppid = getppid();` |

### 2. 文件操作

| 系统调用 | 功能 | 示例 |
|---------|------|------|
| `open()` | 打开文件 | `int fd = open("file.txt", O_RDONLY);` |
| `close()` | 关闭文件 | `close(fd);` |
| `read()` | 读取文件 | `read(fd, buffer, size);` |
| `write()` | 写入文件 | `write(fd, buffer, size);` |
| `lseek()` | 移动文件指针 | `lseek(fd, offset, SEEK_SET);` |
| `stat()` | 获取文件状态 | `stat("file.txt", &st);` |

### 3. 内存管理

| 系统调用 | 功能 | 示例 |
|---------|------|------|
| `brk()` | 调整堆内存 | `brk(end_addr);` |
| `mmap()` | 内存映射 | `mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);` |
| `munmap()` | 解除内存映射 | `munmap(addr, size);` |

### 4. 网络通信

| 系统调用 | 功能 | 示例 |
|---------|------|------|
| `socket()` | 创建套接字 | `int sockfd = socket(AF_INET, SOCK_STREAM, 0);` |
| `bind()` | 绑定地址 | `bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));` |
| `listen()` | 监听连接 | `listen(sockfd, backlog);` |
| `accept()` | 接受连接 | `int connfd = accept(sockfd, NULL, NULL);` |
| `connect()` | 发起连接 | `connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));` |
| `send()` | 发送数据 | `send(connfd, buffer, size, 0);` |
| `recv()` | 接收数据 | `recv(connfd, buffer, size, 0);` |

## 五、并发编程

### 1. 线程同步

#### 1.1 互斥锁

```cpp
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx;
int shared_data = 0;

void increment() {
    for (int i = 0; i < 1000000; i++) {
        lock_guard<mutex> lock(mtx);
        shared_data++;
    }
}

int main() {
    thread t1(increment);
    thread t2(increment);
    
    t1.join();
    t2.join();
    
    cout << "Final shared_data: " << shared_data << endl;
    return 0;
}
```

#### 1.2 读写锁

```cpp
#include <iostream>
#include <thread>
#include <shared_mutex>
using namespace std;

shared_mutex rw_mutex;
int shared_data = 0;

void reader(int id) {
    for (int i = 0; i < 5; i++) {
        {   
            shared_lock<shared_mutex> lock(rw_mutex);
            cout << "Reader " << id << " read: " << shared_data << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void writer(int id) {
    for (int i = 0; i < 5; i++) {
        {   
            unique_lock<shared_mutex> lock(rw_mutex);
            shared_data++;
            cout << "Writer " << id << " wrote: " << shared_data << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    vector<thread> threads;
    
    // 创建3个读者
    for (int i = 0; i < 3; i++) {
        threads.emplace_back(reader, i);
    }
    
    // 创建2个写者
    for (int i = 0; i < 2; i++) {
        threads.emplace_back(writer, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    return 0;
}
```

#### 1.3 信号量

```cpp
#include <iostream>
#include <thread>
#include <semaphore>
using namespace std;

counting_semaphore<5> sem(3); // 最多3个线程同时访问
int shared_data = 0;

void worker(int id) {
    for (int i = 0; i < 3; i++) {
        sem.acquire();
        cout << "Worker " << id << " acquired semaphore, shared_data: " << shared_data << endl;
        shared_data++;
        this_thread::sleep_for(chrono::milliseconds(100));
        sem.release();
        cout << "Worker " << id << " released semaphore" << endl;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main() {
    vector<thread> threads;
    
    for (int i = 0; i < 5; i++) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "Final shared_data: " << shared_data << endl;
    return 0;
}
```

### 2. 死锁及避免

#### 2.1 死锁示例

```cpp
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx1, mtx2;

void thread1() {
    cout << "Thread 1: Locking mtx1" << endl;
    lock_guard<mutex> lock1(mtx1);
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Thread 1: Locking mtx2" << endl;
    lock_guard<mutex> lock2(mtx2);
    cout << "Thread 1: Both locks acquired" << endl;
}

void thread2() {
    cout << "Thread 2: Locking mtx2" << endl;
    lock_guard<mutex> lock2(mtx2);
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Thread 2: Locking mtx1" << endl;
    lock_guard<mutex> lock1(mtx1);
    cout << "Thread 2: Both locks acquired" << endl;
}

int main() {
    thread t1(thread1);
    thread t2(thread2);
    
    t1.join();
    t2.join();
    
    return 0;
}
```

#### 2.2 死锁避免

```cpp
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx1, mtx2;

void thread1() {
    cout << "Thread 1: Locking both mutexes" << endl;
    // 使用lock同时获取多个锁
    lock(mtx1, mtx2);
    lock_guard<mutex> lock1(mtx1, adopt_lock);
    lock_guard<mutex> lock2(mtx2, adopt_lock);
    cout << "Thread 1: Both locks acquired" << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
}

void thread2() {
    cout << "Thread 2: Locking both mutexes" << endl;
    // 按照相同的顺序获取锁
    lock_guard<mutex> lock1(mtx1);
    lock_guard<mutex> lock2(mtx2);
    cout << "Thread 2: Both locks acquired" << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
}

int main() {
    thread t1(thread1);
    thread t2(thread2);
    
    t1.join();
    t2.join();
    
    return 0;
}
```

## 六、常见考点

### 1. 进程与线程的区别

| 特性 | 进程 | 线程 |
|------|------|------|
| 地址空间 | 独立 | 共享 |
| 资源 | 独立 | 共享 |
| 切换开销 | 大 | 小 |
| 通信方式 | 复杂（管道、共享内存等） | 简单（共享内存） |
| 并发度 | 低 | 高 |
| 稳定性 | 高（一个进程崩溃不影响其他） | 低（一个线程崩溃影响整个进程） |

### 2. 进程通信方式

1. **管道**：半双工，只能在父子进程间使用
2. **命名管道**：可以在任意进程间使用
3. **信号**：用于通知进程发生了某事件
4. **消息队列**：可以在进程间传递消息
5. **共享内存**：最快的进程通信方式
6. **信号量**：用于同步进程
7. **套接字**：可以在不同机器间通信

### 3. 虚拟内存的作用

1. **扩大地址空间**：让进程认为自己拥有连续的内存空间
2. **内存保护**：防止进程访问其他进程的内存
3. **内存共享**：多个进程可以共享同一块物理内存
4. **内存管理**：通过分页机制，提高内存利用率
5. **按需加载**：只加载当前需要的内存页

### 4. 页面置换算法

1. **FIFO**：先进先出，可能产生 Belady 异常
2. **LRU**：最近最少使用，性能较好但实现复杂
3. **OPT**：最优置换，理论最优但无法实现
4. **Clock**：近似 LRU，实现简单

### 5. 死锁的必要条件

1. **互斥条件**：资源不能被多个进程同时使用
2. **持有并等待**：进程持有资源并等待其他资源
3. **不可剥夺**：资源只能由持有它的进程主动释放
4. **循环等待**：存在进程循环等待资源的链

### 6. 死锁的处理方法

1. **预防**：破坏死锁的必要条件
2. **避免**：使用银行家算法等避免死锁
3. **检测与恢复**：检测死锁并采取措施恢复
4. **忽略**：鸵鸟算法，适用于死锁发生概率低的系统

## 七、实战题目

### 1. 进程创建与通信

**题目描述**：创建两个子进程，子进程1向子进程2发送消息，子进程2接收并打印消息。

**代码**：

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
using namespace std;

int main() {
    int pipefd1[2], pipefd2[2];
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        cerr << "Pipe creation failed" << endl;
        return 1;
    }
    
    pid_t pid1 = fork();
    if (pid1 < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    } else if (pid1 == 0) {
        // 子进程1：发送消息
        close(pipefd1[0]); // 关闭读端
        close(pipefd2[1]); // 关闭写端
        
        const char* message = "Hello from child 1";
        write(pipefd1[1], message, strlen(message));
        cout << "Child 1 sent: " << message << endl;
        
        // 等待子进程2的响应
        char buffer[1024];
        ssize_t bytes_read = read(pipefd2[0], buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            cout << "Child 1 received: " << buffer << endl;
        }
        
        close(pipefd1[1]);
        close(pipefd2[0]);
        return 0;
    }
    
    pid_t pid2 = fork();
    if (pid2 < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    } else if (pid2 == 0) {
        // 子进程2：接收消息
        close(pipefd1[1]); // 关闭写端
        close(pipefd2[0]); // 关闭读端
        
        char buffer[1024];
        ssize_t bytes_read = read(pipefd1[0], buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            cout << "Child 2 received: " << buffer << endl;
        }
        
        // 发送响应
        const char* response = "Hello from child 2";
        write(pipefd2[1], response, strlen(response));
        cout << "Child 2 sent: " << response << endl;
        
        close(pipefd1[0]);
        close(pipefd2[1]);
        return 0;
    }
    
    // 父进程
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);
    
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    
    cout << "Parent process finished" << endl;
    return 0;
}
```

### 2. 线程池实现

**题目描述**：实现一个简单的线程池，支持添加任务和等待所有任务完成。

**代码**：

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <future>
using namespace std;

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop;
    int active_tasks;
public:
    ThreadPool(size_t threads) : stop(false), active_tasks(0) {
        for (size_t i = 0; i < threads; i++) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(this->mtx);
                        this->cv.wait(lock, [this] { 
                            return this->stop || !this->tasks.empty(); 
                        });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        task = move(this->tasks.front());
                        this->tasks.pop();
                        this->active_tasks++;
                    }
                    task();
                    {
                        unique_lock<mutex> lock(this->mtx);
                        this->active_tasks--;
                        this->cv.notify_one();
                    }
                }
            });
        }
    }
    
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        future<return_type> res = task->get_future();
        {
            unique_lock<mutex> lock(mtx);
            if (stop) {
                throw runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task]() { (*task)(); });
        }
        cv.notify_one();
        return res;
    }
    
    void wait() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this] { return tasks.empty() && active_tasks == 0; });
    }
    
    ~ThreadPool() {
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (thread &worker : workers) {
            worker.join();
        }
    }
};

// 测试代码
int main() {
    ThreadPool pool(4);
    vector<future<int>> results;
    
    for (int i = 0; i < 8; i++) {
        results.emplace_back(
            pool.enqueue([i] {
                cout << "Task " << i << " started" << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Task " << i << " finished" << endl;
                return i * i;
            })
        );
    }
    
    pool.wait();
    
    cout << "\nAll tasks completed" << endl;
    for (size_t i = 0; i < results.size(); i++) {
        cout << "Task " << i << " result: " << results[i].get() << endl;
    }
    
    return 0;
}
```

## 八、总结

操作系统是计算机系统的核心，掌握操作系统相关知识对于理解计算机工作原理和编写高效的C++程序至关重要。通过本文档的学习，您应该能够：

1. **理解进程与线程**：掌握进程创建、通信、线程同步等核心概念
2. **熟悉内存管理**：了解内存分配、虚拟内存、内存映射等机制
3. **掌握文件系统**：学会文件操作和I/O多路复用技术
4. **了解系统调用**：熟悉常见系统调用的使用场景
5. **掌握并发编程**：理解线程同步、死锁及避免方法
6. **应对常见考点**：掌握操作系统面试中的高频问题

在准备校招笔试面试时，建议结合实际代码练习，加深对操作系统概念的理解。通过系统的学习和实践，您一定能够在操作系统相关的面试中取得优异的成绩。