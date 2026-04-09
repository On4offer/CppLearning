# C++数据库编程实战

本文档涵盖数据库编程的核心知识点，包括SQL基础、数据库连接、性能优化等内容，结合C++代码示例，帮助您在校招笔试面试中掌握数据库编程相关知识。

## 一、SQL基础

### 1. 基本SQL语句

#### 1.1 数据定义语言 (DDL)

```sql
-- 创建表
CREATE TABLE users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE,
    age INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 修改表
ALTER TABLE users ADD COLUMN phone VARCHAR(20);

-- 删除表
DROP TABLE users;

-- 创建索引
CREATE INDEX idx_email ON users(email);
```

#### 1.2 数据操作语言 (DML)

```sql
-- 插入数据
INSERT INTO users (name, email, age) VALUES ('Alice', 'alice@example.com', 25);

-- 更新数据
UPDATE users SET age = 26 WHERE id = 1;

-- 删除数据
DELETE FROM users WHERE id = 1;

-- 查询数据
SELECT * FROM users;
SELECT name, email FROM users WHERE age > 20 ORDER BY age DESC;
```

#### 1.3 数据查询语言 (DQL)

```sql
-- 基本查询
SELECT * FROM users;

-- 条件查询
SELECT * FROM users WHERE age BETWEEN 20 AND 30;

-- 排序
SELECT * FROM users ORDER BY created_at DESC;

-- 分组
SELECT age, COUNT(*) FROM users GROUP BY age;

-- 连接查询
SELECT u.name, o.order_date FROM users u JOIN orders o ON u.id = o.user_id;

-- 子查询
SELECT * FROM users WHERE age > (SELECT AVG(age) FROM users);
```

### 2. 事务

```sql
-- 开始事务
START TRANSACTION;

-- 执行操作
INSERT INTO accounts (user_id, balance) VALUES (1, 1000);
UPDATE accounts SET balance = balance - 100 WHERE user_id = 1;
INSERT INTO transactions (from_user, to_user, amount) VALUES (1, 2, 100);
UPDATE accounts SET balance = balance + 100 WHERE user_id = 2;

-- 提交事务
COMMIT;

-- 回滚事务
-- ROLLBACK;
```

## 二、数据库连接

### 1. MySQL连接

#### 1.1 使用MySQL C API

```cpp
#include <iostream>
#include <mysql/mysql.h>
using namespace std;

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    // 初始化MySQL连接
    conn = mysql_init(nullptr);
    if (!conn) {
        cerr << "mysql_init failed" << endl;
        return 1;
    }
    
    // 连接数据库
    if (!mysql_real_connect(conn, "localhost", "root", "password", "testdb", 3306, nullptr, 0)) {
        cerr << "mysql_real_connect failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 1;
    }
    
    // 执行查询
    if (mysql_query(conn, "SELECT * FROM users")) {
        cerr << "mysql_query failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 1;
    }
    
    // 获取结果
    res = mysql_store_result(conn);
    if (!res) {
        cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 1;
    }
    
    // 输出结果
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            cout << (row[i] ? row[i] : "NULL") << " ";
        }
        cout << endl;
    }
    
    // 释放资源
    mysql_free_result(res);
    mysql_close(conn);
    
    return 0;
}
```

#### 1.2 使用MySQL Connector/C++

```cpp
#include <iostream>
#include <mysqlx/xdevapi.h>
using namespace std;
using namespace mysqlx;

int main() {
    try {
        // 连接数据库
        Session session("localhost", 33060, "root", "password");
        
        // 获取 schema
        Schema db = session.getSchema("testdb");
        
        // 获取表
        Table users = db.getTable("users");
        
        // 插入数据
        users.insert("name", "email", "age")
             .values("Bob", "bob@example.com", 30)
             .execute();
        
        // 查询数据
        RowResult result = users.select("*").execute();
        
        // 输出结果
        cout << "ID\tName\tEmail\tAge" << endl;
        cout << "---------------------------" << endl;
        for (auto row : result) {
            cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\t" << row[3] << endl;
        }
        
    } catch (const mysqlx::Error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
```

### 2. SQLite连接

```cpp
#include <iostream>
#include <sqlite3.h>
using namespace std;

static int callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << " ";
    }
    cout << endl;
    return 0;
}

int main() {
    sqlite3 *db;
    char *zErrMsg = nullptr;
    int rc;
    
    // 打开数据库
    rc = sqlite3_open("test.db", &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    }
    
    // 创建表
    const char *sql_create = "CREATE TABLE IF NOT EXISTS users ("  
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, " 
                           "name TEXT NOT NULL, " 
                           "email TEXT UNIQUE, " 
                           "age INTEGER);";
    
    rc = sqlite3_exec(db, sql_create, callback, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    
    // 插入数据
    const char *sql_insert = "INSERT INTO users (name, email, age) VALUES ('Alice', 'alice@example.com', 25);";
    rc = sqlite3_exec(db, sql_insert, callback, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    
    // 查询数据
    const char *sql_select = "SELECT * FROM users;";
    rc = sqlite3_exec(db, sql_select, callback, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    
    // 关闭数据库
    sqlite3_close(db);
    
    return 0;
}
```

### 3. PostgreSQL连接

```cpp
#include <iostream>
#include <libpq-fe.h>
using namespace std;

int main() {
    PGconn *conn;
    PGresult *res;
    
    // 连接数据库
    conn = PQconnectdb("host=localhost port=5432 dbname=testdb user=postgres password=password");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return 1;
    }
    
    // 执行查询
    res = PQexec(conn, "SELECT * FROM users");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "SELECT failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    
    // 输出结果
    int nFields = PQnfields(res);
    for (int i = 0; i < nFields; i++) {
        cout << PQfname(res, i) << "\t";
    }
    cout << endl;
    
    int nRows = PQntuples(res);
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nFields; j++) {
            cout << PQgetvalue(res, i, j) << "\t";
        }
        cout << endl;
    }
    
    // 释放资源
    PQclear(res);
    PQfinish(conn);
    
    return 0;
}
```

## 三、数据库设计

### 1. 范式

1. **第一范式 (1NF)**：确保每列都是原子的，不可再分
2. **第二范式 (2NF)**：满足1NF，且非主键列完全依赖于主键
3. **第三范式 (3NF)**：满足2NF，且非主键列不传递依赖于主键
4. **BCNF**：满足3NF，且所有决定因素都包含主键

### 2. ER图

**实体**：用户、订单、产品
**关系**：用户 - 订单 (一对多)，订单 - 产品 (多对多)

### 3. 表结构设计

```sql
-- 用户表
CREATE TABLE users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 产品表
CREATE TABLE products (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    stock INT DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 订单表
CREATE TABLE orders (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    total_amount DECIMAL(10, 2) NOT NULL,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

-- 订单详情表
CREATE TABLE order_items (
    id INT PRIMARY KEY AUTO_INCREMENT,
    order_id INT NOT NULL,
    product_id INT NOT NULL,
    quantity INT NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (order_id) REFERENCES orders(id),
    FOREIGN KEY (product_id) REFERENCES products(id)
);
```

## 四、性能优化

### 1. SQL语句优化

1. **使用索引**：
   - 为经常查询的列创建索引
   - 避免在索引列上使用函数
   - 合理使用复合索引

2. **优化查询**：
   - 只查询需要的列
   - 使用 LIMIT 限制结果集
   - 避免使用 SELECT *
   - 优化 JOIN 操作

3. **使用预处理语句**：
   - 防止 SQL 注入
   - 提高性能

### 2. 索引优化

```sql
-- 创建索引
CREATE INDEX idx_users_email ON users(email);
CREATE INDEX idx_orders_user_id ON orders(user_id);
CREATE INDEX idx_order_items_order_product ON order_items(order_id, product_id);

-- 查看索引
SHOW INDEX FROM users;

-- 分析索引使用
EXPLAIN SELECT * FROM users WHERE email = 'alice@example.com';
```

### 3. 连接池

```cpp
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <mysql/mysql.h>
using namespace std;

class ConnectionPool {
private:
    queue<MYSQL*> connections;
    mutex mtx;
    condition_variable cv;
    int max_connections;
    string host;
    string user;
    string password;
    string database;
    int port;

public:
    ConnectionPool(int max_conn, const string& h, const string& u, const string& pwd, const string& db, int p) 
        : max_connections(max_conn), host(h), user(u), password(pwd), database(db), port(p) {
        // 初始化连接池
        for (int i = 0; i < max_connections; i++) {
            MYSQL* conn = mysql_init(nullptr);
            if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0)) {
                connections.push(conn);
            } else {
                cerr << "Failed to create connection: " << mysql_error(conn) << endl;
                mysql_close(conn);
            }
        }
    }

    ~ConnectionPool() {
        while (!connections.empty()) {
            MYSQL* conn = connections.front();
            connections.pop();
            mysql_close(conn);
        }
    }

    MYSQL* getConnection() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this] { return !connections.empty(); });
        MYSQL* conn = connections.front();
        connections.pop();
        return conn;
    }

    void releaseConnection(MYSQL* conn) {
        unique_lock<mutex> lock(mtx);
        connections.push(conn);
        cv.notify_one();
    }
};

// 测试代码
int main() {
    ConnectionPool pool(5, "localhost", "root", "password", "testdb", 3306);
    
    // 获取连接
    MYSQL* conn = pool.getConnection();
    
    // 执行查询
    if (mysql_query(conn, "SELECT * FROM users")) {
        cerr << "mysql_query failed: " << mysql_error(conn) << endl;
    } else {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            int num_fields = mysql_num_fields(res);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << (row[i] ? row[i] : "NULL") << " ";
                }
                cout << endl;
            }
            mysql_free_result(res);
        }
    }
    
    // 释放连接
    pool.releaseConnection(conn);
    
    return 0;
}
```

## 五、NoSQL数据库

### 1. Redis

```cpp
#include <iostream>
#include <hiredis/hiredis.h>
using namespace std;

int main() {
    // 连接Redis
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == nullptr || c->err) {
        if (c) {
            cerr << "Redis connection error: " << c->errstr << endl;
            redisFree(c);
        } else {
            cerr << "Redis connection error: can't allocate redis context" << endl;
        }
        return 1;
    }
    
    // 设置键值对
    redisReply *reply = (redisReply*)redisCommand(c, "SET name %s", "Alice");
    cout << "SET name: " << reply->str << endl;
    freeReplyObject(reply);
    
    // 获取值
    reply = (redisReply*)redisCommand(c, "GET name");
    cout << "GET name: " << reply->str << endl;
    freeReplyObject(reply);
    
    // 列表操作
    reply = (redisReply*)redisCommand(c, "LPUSH users %s", "Bob");
    freeReplyObject(reply);
    reply = (redisReply*)redisCommand(c, "LPUSH users %s", "Charlie");
    freeReplyObject(reply);
    
    // 获取列表
    reply = (redisReply*)redisCommand(c, "LRANGE users 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        cout << "Users: " << endl;
        for (int i = 0; i < reply->elements; i++) {
            cout << i << ": " << reply->element[i]->str << endl;
        }
    }
    freeReplyObject(reply);
    
    // 哈希操作
    reply = (redisReply*)redisCommand(c, "HSET user:1 name %s age %d", "Alice", 25);
    freeReplyObject(reply);
    
    // 获取哈希
    reply = (redisReply*)redisCommand(c, "HGETALL user:1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        cout << "User 1: " << endl;
        for (int i = 0; i < reply->elements; i += 2) {
            cout << reply->element[i]->str << ": " << reply->element[i+1]->str << endl;
        }
    }
    freeReplyObject(reply);
    
    // 关闭连接
    redisFree(c);
    
    return 0;
}
```

### 2. MongoDB

```cpp
#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
using namespace std;
using namespace mongocxx;

int main() {
    // 初始化MongoDB实例
    instance inst;
    
    // 创建客户端
    client conn{uri{"mongodb://localhost:27017"}};
    
    // 获取数据库
    database db = conn["testdb"];
    
    // 获取集合
    collection coll = db["users"];
    
    // 插入文档
    bsoncxx::document::value doc = bsoncxx::builder::stream::document
        << "name" << "Alice"
        << "age" << 25
        << "email" << "alice@example.com"
        << bsoncxx::builder::stream::finalize;
    coll.insert_one(doc.view());
    
    // 查询文档
    auto cursor = coll.find({});
    cout << "Users: " << endl;
    for (auto&& doc : cursor) {
        cout << bsoncxx::to_json(doc) << endl;
    }
    
    // 更新文档
    coll.update_one(
        bsoncxx::builder::stream::document << "name" << "Alice" << bsoncxx::builder::stream::finalize,
        bsoncxx::builder::stream::document << "$set" << bsoncxx::builder::stream::open_document << "age" << 26 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize
    );
    
    // 删除文档
    coll.delete_one(bsoncxx::builder::stream::document << "name" << "Alice" << bsoncxx::builder::stream::finalize);
    
    return 0;
}
```

## 六、常见考点

### 1. 事务的ACID特性

- **原子性 (Atomicity)**：事务是一个不可分割的工作单位，要么全部执行，要么全部不执行
- **一致性 (Consistency)**：事务执行前后，数据库从一个一致性状态转变为另一个一致性状态
- **隔离性 (Isolation)**：多个事务并发执行时，一个事务的执行不应影响其他事务的执行
- **持久性 (Durability)**：事务一旦提交，其结果应该永久保存在数据库中

### 2. 索引类型

- **B树索引**：适用于范围查询和排序
- **哈希索引**：适用于等值查询，不支持范围查询
- **全文索引**：适用于文本搜索
- **空间索引**：适用于地理空间数据

### 3. 数据库锁机制

- **共享锁 (S锁)**：读锁，多个事务可以同时读取同一资源
- **排他锁 (X锁)**：写锁，只有一个事务可以修改同一资源
- **意向锁**：表示事务将要对表中的行加锁

### 4. 隔离级别

| 隔离级别 | 脏读 | 不可重复读 | 幻读 |
|---------|------|-----------|------|
| 读未提交 (Read Uncommitted) | 可能 | 可能 | 可能 |
| 读已提交 (Read Committed) | 不可能 | 可能 | 可能 |
| 可重复读 (Repeatable Read) | 不可能 | 不可能 | 可能 |
| 串行化 (Serializable) | 不可能 | 不可能 | 不可能 |

### 5. 数据库优化策略

- **SQL优化**：优化查询语句，使用索引
- **索引优化**：合理创建和使用索引
- **硬件优化**：增加内存，使用SSD
- **架构优化**：读写分离，分库分表
- **缓存优化**：使用Redis等缓存

## 七、实战题目

### 1. 学生管理系统

**题目描述**：实现一个学生管理系统，包含学生信息的增删改查功能。

**代码**：

```cpp
#include <iostream>
#include <mysql/mysql.h>
#include <string>
using namespace std;

class StudentManager {
private:
    MYSQL *conn;

public:
    StudentManager(const string& host, const string& user, const string& password, const string& database) {
        conn = mysql_init(nullptr);
        if (!conn) {
            throw runtime_error("mysql_init failed");
        }
        if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), database.c_str(), 3306, nullptr, 0)) {
            string error = mysql_error(conn);
            mysql_close(conn);
            throw runtime_error("mysql_real_connect failed: " + error);
        }
        
        // 创建学生表
        const char* create_table = "CREATE TABLE IF NOT EXISTS students (" 
                                 "id INT PRIMARY KEY AUTO_INCREMENT, " 
                                 "name VARCHAR(50) NOT NULL, " 
                                 "age INT, " 
                                 "gender VARCHAR(10), " 
                                 "major VARCHAR(100));";
        if (mysql_query(conn, create_table)) {
            string error = mysql_error(conn);
            mysql_close(conn);
            throw runtime_error("create table failed: " + error);
        }
    }
    
    ~StudentManager() {
        if (conn) {
            mysql_close(conn);
        }
    }
    
    void addStudent(const string& name, int age, const string& gender, const string& major) {
        char query[512];
        snprintf(query, sizeof(query), "INSERT INTO students (name, age, gender, major) VALUES ('%s', %d, '%s', '%s')", 
                 name.c_str(), age, gender.c_str(), major.c_str());
        
        if (mysql_query(conn, query)) {
            throw runtime_error("add student failed: " + string(mysql_error(conn)));
        }
        cout << "Student added successfully!" << endl;
    }
    
    void deleteStudent(int id) {
        char query[256];
        snprintf(query, sizeof(query), "DELETE FROM students WHERE id = %d", id);
        
        if (mysql_query(conn, query)) {
            throw runtime_error("delete student failed: " + string(mysql_error(conn)));
        }
        cout << "Student deleted successfully!" << endl;
    }
    
    void updateStudent(int id, const string& name, int age, const string& gender, const string& major) {
        char query[512];
        snprintf(query, sizeof(query), "UPDATE students SET name = '%s', age = %d, gender = '%s', major = '%s' WHERE id = %d", 
                 name.c_str(), age, gender.c_str(), major.c_str(), id);
        
        if (mysql_query(conn, query)) {
            throw runtime_error("update student failed: " + string(mysql_error(conn)));
        }
        cout << "Student updated successfully!" << endl;
    }
    
    void queryStudents() {
        if (mysql_query(conn, "SELECT * FROM students")) {
            throw runtime_error("query students failed: " + string(mysql_error(conn)));
        }
        
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res) {
            throw runtime_error("mysql_store_result failed: " + string(mysql_error(conn)));
        }
        
        int num_fields = mysql_num_fields(res);
        MYSQL_FIELD *fields = mysql_fetch_fields(res);
        
        // 打印表头
        for (int i = 0; i < num_fields; i++) {
            cout << fields[i].name << "\t";
        }
        cout << endl;
        cout << "-------------------------------------------" << endl;
        
        // 打印数据
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                cout << (row[i] ? row[i] : "NULL") << "\t";
            }
            cout << endl;
        }
        
        mysql_free_result(res);
    }
};

int main() {
    try {
        StudentManager manager("localhost", "root", "password", "testdb");
        
        // 添加学生
        manager.addStudent("Alice", 20, "Female", "Computer Science");
        manager.addStudent("Bob", 21, "Male", "Electrical Engineering");
        
        // 查询学生
        cout << "\nAll students:" << endl;
        manager.queryStudents();
        
        // 更新学生
        manager.updateStudent(1, "Alice Smith", 20, "Female", "Software Engineering");
        
        // 查询学生
        cout << "\nAfter update:" << endl;
        manager.queryStudents();
        
        // 删除学生
        manager.deleteStudent(2);
        
        // 查询学生
        cout << "\nAfter delete:" << endl;
        manager.queryStudents();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
```

### 2. 商品库存管理

**题目描述**：实现一个商品库存管理系统，包含商品的增删改查和库存管理功能。

**代码**：

```cpp
#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

class InventoryManager {
private:
    sqlite3 *db;

public:
    InventoryManager(const string& db_name) {
        int rc = sqlite3_open(db_name.c_str(), &db);
        if (rc) {
            throw runtime_error("Can't open database: " + string(sqlite3_errmsg(db)));
        }
        
        // 创建商品表
        const char* create_table = "CREATE TABLE IF NOT EXISTS products ("  
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, " 
                                 "name TEXT NOT NULL, " 
                                 "price REAL NOT NULL, " 
                                 "stock INTEGER DEFAULT 0, " 
                                 "description TEXT);";
        
        char *zErrMsg = nullptr;
        rc = sqlite3_exec(db, create_table, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) {
            string error = zErrMsg;
            sqlite3_free(zErrMsg);
            sqlite3_close(db);
            throw runtime_error("create table failed: " + error);
        }
    }
    
    ~InventoryManager() {
        if (db) {
            sqlite3_close(db);
        }
    }
    
    void addProduct(const string& name, double price, int stock, const string& description) {
        char sql[512];
        snprintf(sql, sizeof(sql), "INSERT INTO products (name, price, stock, description) VALUES ('%s', %.2f, %d, '%s')", 
                 name.c_str(), price, stock, description.c_str());
        
        char *zErrMsg = nullptr;
        int rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) {
            string error = zErrMsg;
            sqlite3_free(zErrMsg);
            throw runtime_error("add product failed: " + error);
        }
        cout << "Product added successfully!" << endl;
    }
    
    void updateStock(int id, int quantity) {
        char sql[256];
        snprintf(sql, sizeof(sql), "UPDATE products SET stock = stock + %d WHERE id = %d", quantity, id);
        
        char *zErrMsg = nullptr;
        int rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) {
            string error = zErrMsg;
            sqlite3_free(zErrMsg);
            throw runtime_error("update stock failed: " + error);
        }
        cout << "Stock updated successfully!" << endl;
    }
    
    void queryProducts() {
        const char* sql = "SELECT * FROM products";
        
        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            throw runtime_error("prepare statement failed: " + string(sqlite3_errmsg(db)));
        }
        
        cout << "ID\tName\tPrice\tStock\tDescription" << endl;
        cout << "-------------------------------------------" << endl;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = (const char*)sqlite3_column_text(stmt, 1);
            double price = sqlite3_column_double(stmt, 2);
            int stock = sqlite3_column_int(stmt, 3);
            const char* description = (const char*)sqlite3_column_text(stmt, 4);
            
            cout << id << "\t" << name << "\t" << price << "\t" << stock << "\t" << (description ? description : "") << endl;
        }
        
        sqlite3_finalize(stmt);
    }
};

int main() {
    try {
        InventoryManager manager("inventory.db");
        
        // 添加商品
        manager.addProduct("iPhone 13", 7999.99, 50, "Apple smartphone");
        manager.addProduct("MacBook Pro", 12999.99, 20, "Apple laptop");
        
        // 查询商品
        cout << "\nAll products:" << endl;
        manager.queryProducts();
        
        // 更新库存
        manager.updateStock(1, -5); // 减少5个库存
        manager.updateStock(2, 10); // 增加10个库存
        
        // 查询商品
        cout << "\nAfter stock update:" << endl;
        manager.queryProducts();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
```

## 八、总结

数据库编程是C++开发中的重要组成部分，掌握数据库编程相关知识对于构建数据驱动的应用程序至关重要。通过本文档的学习，您应该能够：

1. **掌握SQL基础**：熟悉SQL语句的使用，包括DDL、DML和DQL
2. **连接数据库**：使用不同的数据库API连接MySQL、SQLite和PostgreSQL
3. **设计数据库**：理解数据库设计原则，包括范式、ER图和表结构设计
4. **优化数据库性能**：掌握SQL语句优化、索引优化和连接池技术
5. **使用NoSQL数据库**：了解Redis和MongoDB的基本使用
6. **应对常见考点**：熟悉数据库面试中的高频问题

在准备校招笔试面试时，建议结合实际项目经验，加深对数据库编程概念的理解。通过系统的学习和实践，您一定能够在数据库编程相关的面试中取得优异的成绩。