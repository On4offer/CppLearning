# C++网络编程核心指南

本文档涵盖网络编程的核心知识点，包括网络协议、socket编程、HTTP协议、网络安全等内容，结合C++代码示例，帮助您在校招笔试面试中掌握网络编程相关知识。

## 一、网络协议基础

### 1. TCP/IP协议栈

| 层次 | 协议 | 功能 |
|------|------|------|
| 应用层 | HTTP、FTP、SMTP、DNS | 提供应用服务 |
| 传输层 | TCP、UDP | 端到端通信 |
| 网络层 | IP、ICMP、ARP | 路由和寻址 |
| 数据链路层 | Ethernet、WiFi | 物理介质访问 |
| 物理层 | 网线、光纤 | 物理传输 |

### 2. TCP协议

#### 2.1 TCP三次握手

```
客户端          服务器
  |              |
  |----SYN-----> |
  |              |
  |<---SYN+ACK---|
  |              |
  |----ACK-----> |
  |              |
  |     数据传输     |
  |              |
```

#### 2.2 TCP四次挥手

```
客户端          服务器
  |              |
  |----FIN-----> |
  |              |
  |<---ACK------|
  |              |
  |<---FIN------|
  |              |
  |----ACK-----> |
  |              |
```

### 3. UDP协议

- **无连接**：不需要建立连接
- **不可靠**：不保证数据传输可靠性
- **面向报文**：保留报文边界
- **速度快**：适用于实时应用

## 二、Socket编程

### 1. TCP服务器

```cpp
#include <iostream>
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
    const char* hello = "Hello from server";
    
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
    
    cout << "Server listening on port 8080..." << endl;
    
    // 接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    // 读取客户端消息
    int valread = read(new_socket, buffer, 1024);
    cout << "Client: " << buffer << endl;
    
    // 发送消息给客户端
    send(new_socket, hello, strlen(hello), 0);
    cout << "Hello message sent" << endl;
    
    // 关闭 socket
    close(new_socket);
    close(server_fd);
    
    return 0;
}
```

### 2. TCP客户端

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char* hello = "Hello from client";
    
    // 创建 socket 文件描述符
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    // 转换 IP 地址
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        cout << "Invalid address/ Address not supported" << endl;
        return -1;
    }
    
    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed" << endl;
        return -1;
    }
    
    // 发送消息
    send(sock, hello, strlen(hello), 0);
    cout << "Hello message sent" << endl;
    
    // 读取服务器响应
    int valread = read(sock, buffer, 1024);
    cout << "Server: " << buffer << endl;
    
    // 关闭 socket
    close(sock);
    
    return 0;
}
```

### 3. UDP服务器

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;
    
    // 创建 socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    
    // 绑定 socket
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    cout << "UDP server listening on port 8080..." << endl;
    
    socklen_t len;
    int n;
    
    len = sizeof(cliaddr);
    
    // 接收客户端消息
    n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    cout << "Client: " << buffer << endl;
    
    // 发送消息给客户端
    const char* hello = "Hello from UDP server";
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    cout << "Hello message sent" << endl;
    
    // 关闭 socket
    close(sockfd);
    
    return 0;
}
```

### 4. UDP客户端

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr;
    
    // 创建 socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // 发送消息
    const char* hello = "Hello from UDP client";
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    cout << "Hello message sent" << endl;
    
    // 接收服务器响应
    socklen_t len;
    int n;
    n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    cout << "Server: " << buffer << endl;
    
    // 关闭 socket
    close(sockfd);
    
    return 0;
}
```

## 三、HTTP协议

### 1. HTTP请求格式

```
GET /index.html HTTP/1.1

Host: www.example.com

User-Agent: Mozilla/5.0

Accept: text/html

Connection: keep-alive



```

### 2. HTTP响应格式

```
HTTP/1.1 200 OK

Content-Type: text/html

Content-Length: 123

Server: Apache



<html><body>Hello World</body></html>
```

### 3. HTTP方法

| 方法 | 描述 |
|------|------|
| GET | 获取资源 |
| POST | 提交数据 |
| PUT | 更新资源 |
| DELETE | 删除资源 |
| HEAD | 获取头信息 |
| OPTIONS | 检查服务器支持的方法 |
| PATCH | 部分更新资源 |

### 4. HTTP状态码

| 状态码 | 描述 |
|--------|------|
| 200 | OK |
| 301 | Moved Permanently |
| 400 | Bad Request |
| 401 | Unauthorized |
| 403 | Forbidden |
| 404 | Not Found |
| 500 | Internal Server Error |

### 5. HTTP服务器实现

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
using namespace std;

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    
    // 读取请求
    int valread = read(client_socket, buffer, 1024);
    if (valread < 0) {
        perror("read failed");
        close(client_socket);
        return;
    }
    
    cout << "Received request:\n" << buffer << endl;
    
    // 构建响应
    const char* response = "HTTP/1.1 200 OK\r\n" 
                          "Content-Type: text/html\r\n" 
                          "Content-Length: 20\r\n" 
                          "\r\n" 
                          "<h1>Hello World</h1>";
    
    // 发送响应
    send(client_socket, response, strlen(response), 0);
    
    // 关闭连接
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
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
    
    cout << "HTTP server listening on port 8080..." << endl;
    
    vector<thread> threads;
    
    while (true) {
        // 接受连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // 为每个客户端创建线程
        threads.emplace_back(handle_client, new_socket);
    }
    
    // 等待所有线程结束
    for (auto& t : threads) {
        t.join();
    }
    
    close(server_fd);
    return 0;
}
```

## 四、网络安全

### 1. HTTPS原理

1. **握手过程**：
   - 客户端发送 Client Hello（支持的加密套件）
   - 服务器发送 Server Hello（选择的加密套件）
   - 服务器发送证书
   - 客户端验证证书
   - 客户端生成会话密钥，用服务器公钥加密发送
   - 服务器用私钥解密得到会话密钥
   - 双方使用会话密钥加密通信

2. **加密算法**：
   - 非对称加密：RSA、ECC
   - 对称加密：AES、ChaCha20
   - 哈希算法：SHA-256、SHA-384

### 2. 常见网络攻击

| 攻击类型 | 描述 | 防护措施 |
|---------|------|---------|
| DDoS | 分布式拒绝服务 | CDN、流量清洗、速率限制 |
| SQL注入 | 注入恶意SQL代码 | 参数化查询、输入验证 |
| XSS | 跨站脚本攻击 | 输入过滤、输出编码 |
| CSRF | 跨站请求伪造 | 验证码、token验证 |
| 中间人攻击 | 拦截通信 | HTTPS、证书验证 |

### 3. 安全编程实践

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/err.h>
using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // 初始化 OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    const SSL_METHOD* method = TLS_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    
    // 加载证书和私钥
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    
    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    
    // 创建 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return 1;
    }
    
    // 设置 socket 选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return 1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8443);
    
    // 绑定
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }
    
    // 监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }
    
    cout << "HTTPS server listening on port 8443..." << endl;
    
    while (true) {
        // 接受连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            return 1;
        }
        
        // 创建 SSL 连接
        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, new_socket);
        
        // 进行 SSL 握手
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            close(new_socket);
            continue;
        }
        
        // 读取数据
        char buffer[1024] = {0};
        int valread = SSL_read(ssl, buffer, 1024);
        if (valread > 0) {
            cout << "Received: " << buffer << endl;
        }
        
        // 发送数据
        const char* response = "Hello from HTTPS server";
        SSL_write(ssl, response, strlen(response));
        
        // 关闭连接
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(new_socket);
    }
    
    SSL_CTX_free(ctx);
    close(server_fd);
    return 0;
}
```

## 五、性能优化

### 1. 网络延迟优化

1. **减少往返时间**：
   - 使用 CDN 加速
   - 优化 DNS 解析
   - 使用 HTTP/2 多路复用

2. **减少数据传输量**：
   - 压缩数据（gzip、brotli）
   - 减少 HTTP 头部大小
   - 使用 WebSocket 长连接

3. **优化连接管理**：
   - 连接池
   - 复用连接
   - 适当的超时设置

### 2. 并发连接处理

#### 2.1 多线程服务器

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
using namespace std;

void handle_connection(int client_socket) {
    char buffer[1024] = {0};
    
    while (true) {
        int valread = read(client_socket, buffer, 1024);
        if (valread <= 0) {
            break;
        }
        
        cout << "Received: " << buffer << endl;
        send(client_socket, buffer, valread, 0);
        memset(buffer, 0, sizeof(buffer));
    }
    
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
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
    if (listen(server_fd, 100) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    cout << "Multi-threaded server listening on port 8080..." << endl;
    
    vector<thread> threads;
    
    while (true) {
        // 接受连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // 创建线程处理连接
        threads.emplace_back(handle_connection, new_socket);
    }
    
    // 等待所有线程结束
    for (auto& t : threads) {
        t.join();
    }
    
    close(server_fd);
    return 0;
}
```

#### 2.2 线程池服务器

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

class ThreadPool {
private:
    vector<thread> workers;
    queue<int> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop;
public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; i++) {
            workers.emplace_back([this] {
                while (true) {
                    int client_socket;
                    {
                        unique_lock<mutex> lock(this->mtx);
                        this->cv.wait(lock, [this] { 
                            return this->stop || !this->tasks.empty(); 
                        });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        client_socket = this->tasks.front();
                        this->tasks.pop();
                    }
                    
                    // 处理连接
                    char buffer[1024] = {0};
                    while (true) {
                        int valread = read(client_socket, buffer, 1024);
                        if (valread <= 0) {
                            break;
                        }
                        send(client_socket, buffer, valread, 0);
                        memset(buffer, 0, sizeof(buffer));
                    }
                    close(client_socket);
                }
            });
        }
    }
    
    void enqueue(int client_socket) {
        {
            unique_lock<mutex> lock(mtx);
            tasks.push(client_socket);
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

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
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
    if (listen(server_fd, 100) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // 创建线程池
    ThreadPool pool(4);
    
    cout << "Thread pool server listening on port 8080..." << endl;
    
    while (true) {
        // 接受连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // 添加到线程池
        pool.enqueue(new_socket);
    }
    
    close(server_fd);
    return 0;
}
```

### 3. 负载均衡

1. **负载均衡策略**：
   - 轮询（Round Robin）
   - 权重轮询（Weighted Round Robin）
   - 最少连接（Least Connection）
   - IP哈希（IP Hash）
   - 随机（Random）

2. **负载均衡实现**：
   - 硬件负载均衡器
   - 软件负载均衡器（Nginx、HAProxy）
   - 应用层负载均衡

## 六、常见考点

### 1. TCP与UDP的区别

| 特性 | TCP | UDP |
|------|------|------|
| 连接 | 面向连接 | 无连接 |
| 可靠性 | 可靠 | 不可靠 |
| 传输单位 | 字节流 | 报文 |
| 速度 | 较慢 | 较快 |
| 拥塞控制 | 有 | 无 |
| 流量控制 | 有 | 无 |
| 应用场景 | Web、邮件 | 视频、语音 |

### 2. HTTP/1.1与HTTP/2的区别

| 特性 | HTTP/1.1 | HTTP/2 |
|------|----------|--------|
| 多路复用 | 无（每个连接一个请求） | 有（一个连接多个请求） |
| 头部压缩 | 无 | 有（HPACK） |
| 服务器推送 | 无 | 有 |
| 二进制分帧 | 无 | 有 |
| 优先级 | 无 | 有 |

### 3. HTTPS的工作原理

1. **客户端发起HTTPS请求**
2. **服务器返回证书**
3. **客户端验证证书**
4. **客户端生成会话密钥**
5. **客户端用服务器公钥加密会话密钥**
6. **服务器用私钥解密得到会话密钥**
7. **双方使用会话密钥加密通信**

### 4. 网络编程中的常见错误

| 错误 | 原因 | 解决方案 |
|------|------|----------|
| 连接超时 | 网络问题或服务器无响应 | 检查网络连接，设置合理的超时时间 |
| 连接被拒绝 | 端口未开放或服务未启动 | 检查服务是否运行，端口是否正确 |
| 数据发送失败 | 连接已关闭或网络中断 | 检查连接状态，实现重连机制 |
| 内存泄漏 | 未关闭socket | 确保在适当的时候关闭socket |
| 缓冲区溢出 | 接收数据时未处理边界情况 | 检查缓冲区大小，使用循环读取 |

## 七、实战题目

### 1. 简单的HTTP服务器

**题目描述**：实现一个简单的HTTP服务器，能够处理GET请求并返回静态文件。

**代码**：

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
using namespace std;

string read_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    
    // 读取请求
    int valread = read(client_socket, buffer, 1024);
    if (valread < 0) {
        close(client_socket);
        return;
    }
    
    // 解析请求
    string request(buffer);
    size_t method_end = request.find(' ');
    size_t path_end = request.find(' ', method_end + 1);
    string method = request.substr(0, method_end);
    string path = request.substr(method_end + 1, path_end - method_end - 1);
    
    // 处理GET请求
    if (method == "GET") {
        // 默认路径
        if (path == "/") {
            path = "/index.html";
        }
        
        // 读取文件
        string filename = "." + path;
        string content = read_file(filename);
        
        if (content.empty()) {
            // 404 Not Found
            string response = "HTTP/1.1 404 Not Found\r\n" 
                           "Content-Type: text/html\r\n" 
                           "Content-Length: 13\r\n" 
                           "\r\n" 
                           "404 Not Found";
            send(client_socket, response.c_str(), response.size(), 0);
        } else {
            // 200 OK
            string content_type = "text/html";
            if (path.substr(path.find_last_of('.') + 1) == "css") {
                content_type = "text/css";
            } else if (path.substr(path.find_last_of('.') + 1) == "js") {
                content_type = "application/javascript";
            }
            
            string response = "HTTP/1.1 200 OK\r\n" 
                           "Content-Type: " + content_type + "\r\n" 
                           "Content-Length: " + to_string(content.size()) + "\r\n" 
                           "\r\n" 
                           + content;
            send(client_socket, response.c_str(), response.size(), 0);
        }
    }
    
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
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
    
    cout << "HTTP server listening on port 8080..." << endl;
    
    while (true) {
        // 接受连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // 创建线程处理连接
        thread t(handle_client, new_socket);
        t.detach(); // 分离线程
    }
    
    close(server_fd);
    return 0;
}
```

### 2. 聊天服务器

**题目描述**：实现一个基于TCP的聊天服务器，支持多个客户端同时连接和消息广播。

**代码**：

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

vector<int> clients;
mutex clients_mutex;

void broadcast_message(const string& message, int sender_fd) {
    lock_guard<mutex> lock(clients_mutex);
    for (int client_fd : clients) {
        if (client_fd != sender_fd) {
            send(client_fd, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_fd) {
    char buffer[1024] = {0};
    
    // 添加到客户端列表
    {
        lock_guard<mutex> lock(clients_mutex);
        clients.push_back(client_fd);
    }
    
    cout << "Client connected: " << client_fd << endl;
    
    while (true) {
        int valread = read(client_fd, buffer, 1024);
        if (valread <= 0) {
            break;
        }
        
        string message(buffer, valread);
        cout << "Received: " << message << " from " << client_fd << endl;
        
        // 广播消息
        broadcast_message(message, client_fd);
        memset(buffer, 0, sizeof(buffer));
    }
    
    // 从客户端列表移除
    {
        lock_guard<mutex> lock(clients_mutex);
        clients.erase(remove(clients.begin(), clients.end(), client_fd), clients.end());
    }
    
    cout << "Client disconnected: " << client_fd << endl;
    close(client_fd);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
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
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    cout << "Chat server listening on port 8080..." << endl;
    
    vector<thread> threads;
    
    while (true) {
        // 接受连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // 创建线程处理连接
        threads.emplace_back(handle_client, new_socket);
    }
    
    // 等待所有线程结束
    for (auto& t : threads) {
        t.join();
    }
    
    close(server_fd);
    return 0;
}
```

## 八、总结

网络编程是C++开发中的重要领域，掌握网络编程相关知识对于构建高性能、可靠的网络应用至关重要。通过本文档的学习，您应该能够：

1. **理解网络协议**：掌握TCP/IP协议栈、HTTP协议等核心概念
2. **熟练使用Socket编程**：实现TCP/UDP服务器和客户端
3. **构建HTTP服务**：理解HTTP请求响应格式，实现简单的HTTP服务器
4. **保证网络安全**：了解HTTPS原理和常见网络攻击的防护措施
5. **优化网络性能**：掌握网络延迟优化、并发连接处理和负载均衡技术
6. **应对常见考点**：熟悉网络编程面试中的高频问题

在准备校招笔试面试时，建议结合实际项目经验，加深对网络编程概念的理解。通过系统的学习和实践，您一定能够在网络编程相关的面试中取得优异的成绩。