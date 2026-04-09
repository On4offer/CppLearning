# C++算法与数据结构实战指南

本文档涵盖算法与数据结构的核心知识点，包括常见数据结构的实现、经典算法的应用、复杂度分析以及高频笔试题目，帮助您在校招笔试面试中取得优异成绩。

## 一、常见数据结构

### 1. 链表

#### 1.1 单链表

```cpp
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 遍历链表
void printList(ListNode* head) {
    ListNode* curr = head;
    while (curr) {
        cout << curr->val << " -> ";
        curr = curr->next;
    }
    cout << "nullptr" << endl;
}

// 反转链表
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// 测试代码
int main() {
    // 创建链表: 1->2->3->4->5
    ListNode* head = new ListNode(1);
    ListNode* curr = head;
    for (int i = 2; i <= 5; i++) {
        curr->next = new ListNode(i);
        curr = curr->next;
    }
    
    cout << "原始链表: ";
    printList(head);
    
    ListNode* reversed = reverseList(head);
    cout << "反转后: ";
    printList(reversed);
    
    // 释放内存
    while (reversed) {
        ListNode* temp = reversed;
        reversed = reversed->next;
        delete temp;
    }
    
    return 0;
}
```

#### 1.2 双链表

```cpp
#include <iostream>
using namespace std;

struct DoubleListNode {
    int val;
    DoubleListNode* prev;
    DoubleListNode* next;
    DoubleListNode(int x) : val(x), prev(nullptr), next(nullptr) {}
};

// 遍历链表
void printList(DoubleListNode* head) {
    DoubleListNode* curr = head;
    while (curr) {
        cout << curr->val << " <-> ";
        curr = curr->next;
    }
    cout << "nullptr" << endl;
}

// 插入节点
void insertAfter(DoubleListNode* node, int val) {
    DoubleListNode* newNode = new DoubleListNode(val);
    newNode->prev = node;
    newNode->next = node->next;
    if (node->next) {
        node->next->prev = newNode;
    }
    node->next = newNode;
}

// 测试代码
int main() {
    // 创建双链表: 1<->2<->3
    DoubleListNode* head = new DoubleListNode(1);
    DoubleListNode* node2 = new DoubleListNode(2);
    DoubleListNode* node3 = new DoubleListNode(3);
    
    head->next = node2;
    node2->prev = head;
    node2->next = node3;
    node3->prev = node2;
    
    cout << "原始链表: ";
    printList(head);
    
    // 在节点2后插入4
    insertAfter(node2, 4);
    cout << "插入后: ";
    printList(head);
    
    // 释放内存
    while (head) {
        DoubleListNode* temp = head;
        head = head->next;
        delete temp;
    }
    
    return 0;
}
```

### 2. 栈

```cpp
#include <iostream>
#include <vector>
using namespace std;

class Stack {
private:
    vector<int> data;
public:
    void push(int val) {
        data.push_back(val);
    }
    
    void pop() {
        if (!empty()) {
            data.pop_back();
        }
    }
    
    int top() {
        if (!empty()) {
            return data.back();
        }
        return -1;
    }
    
    bool empty() {
        return data.empty();
    }
    
    int size() {
        return data.size();
    }
};

// 测试代码
int main() {
    Stack s;
    s.push(1);
    s.push(2);
    s.push(3);
    
    cout << "栈顶元素: " << s.top() << endl;
    cout << "栈大小: " << s.size() << endl;
    
    s.pop();
    cout << "弹出后栈顶: " << s.top() << endl;
    
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;
    
    return 0;
}
```

### 3. 队列

```cpp
#include <iostream>
#include <vector>
using namespace std;

class Queue {
private:
    vector<int> data;
    int front;
public:
    Queue() : front(0) {}
    
    void push(int val) {
        data.push_back(val);
    }
    
    void pop() {
        if (!empty()) {
            front++;
        }
    }
    
    int peek() {
        if (!empty()) {
            return data[front];
        }
        return -1;
    }
    
    bool empty() {
        return front >= data.size();
    }
    
    int size() {
        return data.size() - front;
    }
};

// 测试代码
int main() {
    Queue q;
    q.push(1);
    q.push(2);
    q.push(3);
    
    cout << "队首元素: " << q.peek() << endl;
    cout << "队列大小: " << q.size() << endl;
    
    q.pop();
    cout << "弹出后队首: " << q.peek() << endl;
    
    while (!q.empty()) {
        cout << q.peek() << " ";
        q.pop();
    }
    cout << endl;
    
    return 0;
}
```

### 4. 二叉树

```cpp
#include <iostream>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 前序遍历
void preorder(TreeNode* root) {
    if (root) {
        cout << root->val << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

// 中序遍历
void inorder(TreeNode* root) {
    if (root) {
        inorder(root->left);
        cout << root->val << " ";
        inorder(root->right);
    }
}

// 后序遍历
void postorder(TreeNode* root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        cout << root->val << " ";
    }
}

// 层序遍历
void levelorder(TreeNode* root) {
    if (!root) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        cout << node->val << " ";
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

// 测试代码
int main() {
    // 创建二叉树
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    cout << "前序遍历: ";
    preorder(root);
    cout << endl;
    
    cout << "中序遍历: ";
    inorder(root);
    cout << endl;
    
    cout << "后序遍历: ";
    postorder(root);
    cout << endl;
    
    cout << "层序遍历: ";
    levelorder(root);
    cout << endl;
    
    // 释放内存
    // 实际项目中应使用析构函数或递归释放
    return 0;
}
```

## 二、经典算法

### 1. 排序算法

#### 1.1 快速排序

```cpp
#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (nums[j] <= pivot) {
            i++;
            swap(nums[i], nums[j]);
        }
    }
    swap(nums[i + 1], nums[right]);
    return i + 1;
}

void quickSort(vector<int>& nums, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(nums, left, right);
        quickSort(nums, left, pivotIndex - 1);
        quickSort(nums, pivotIndex + 1, right);
    }
}

// 测试代码
int main() {
    vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    cout << "排序前: ";
    for (int num : nums) cout << num << " ";
    cout << endl;
    
    quickSort(nums, 0, nums.size() - 1);
    
    cout << "排序后: ";
    for (int num : nums) cout << num << " ";
    cout << endl;
    
    return 0;
}
```

#### 1.2 归并排序

```cpp
#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int>& nums, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
        }
    }
    while (i <= mid) temp[k++] = nums[i++];
    while (j <= right) temp[k++] = nums[j++];
    for (int p = 0; p < temp.size(); p++) {
        nums[left + p] = temp[p];
    }
}

void mergeSort(vector<int>& nums, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(nums, left, mid);
        mergeSort(nums, mid + 1, right);
        merge(nums, left, mid, right);
    }
}

// 测试代码
int main() {
    vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    cout << "排序前: ";
    for (int num : nums) cout << num << " ";
    cout << endl;
    
    mergeSort(nums, 0, nums.size() - 1);
    
    cout << "排序后: ";
    for (int num : nums) cout << num << " ";
    cout << endl;
    
    return 0;
}
```

### 2. 搜索算法

#### 2.1 二分查找

```cpp
#include <iostream>
#include <vector>
using namespace std;

int binarySearch(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// 测试代码
int main() {
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int target = 5;
    int result = binarySearch(nums, target);
    cout << "元素 " << target << " 在数组中的索引: " << result << endl;
    
    target = 10;
    result = binarySearch(nums, target);
    cout << "元素 " << target << " 在数组中的索引: " << result << endl;
    
    return 0;
}
```

#### 2.2 深度优先搜索 (DFS)

```cpp
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// 递归实现
void dfsRecursive(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    visited[start] = true;
    cout << start << " ";
    for (int neighbor : graph[start]) {
        if (!visited[neighbor]) {
            dfsRecursive(graph, neighbor, visited);
        }
    }
}

// 迭代实现
void dfsIterative(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    stack<int> s;
    s.push(start);
    visited[start] = true;
    
    while (!s.empty()) {
        int node = s.top();
        s.pop();
        cout << node << " ";
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                s.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

// 测试代码
int main() {
    // 构建图: 0-1-2-3, 0-4
    vector<vector<int>> graph = {
        {1, 4},
        {0, 2},
        {1, 3},
        {2},
        {0}
    };
    
    cout << "递归DFS: ";
    vector<bool> visited(graph.size(), false);
    dfsRecursive(graph, 0, visited);
    cout << endl;
    
    cout << "迭代DFS: ";
    dfsIterative(graph, 0);
    cout << endl;
    
    return 0;
}
```

#### 2.3 广度优先搜索 (BFS)

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

// 测试代码
int main() {
    // 构建图: 0-1-2-3, 0-4
    vector<vector<int>> graph = {
        {1, 4},
        {0, 2},
        {1, 3},
        {2},
        {0}
    };
    
    cout << "BFS: ";
    bfs(graph, 0);
    cout << endl;
    
    return 0;
}
```

### 3. 动态规划

#### 3.1 斐波那契数列

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 递归实现
int fibRecursive(int n) {
    if (n <= 1) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// 动态规划实现
int fibDP(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// 空间优化
int fibOptimized(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// 测试代码
int main() {
    int n = 10;
    cout << "递归实现: fib(" << n << ") = " << fibRecursive(n) << endl;
    cout << "动态规划: fib(" << n << ") = " << fibDP(n) << endl;
    cout << "空间优化: fib(" << n << ") = " << fibOptimized(n) << endl;
    
    return 0;
}
```

#### 3.2 最长公共子序列

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int longestCommonSubsequence(string text1, string text2) {
    int m = text1.size(), n = text2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}

// 测试代码
int main() {
    string text1 = "abcde";
    string text2 = "ace";
    cout << "最长公共子序列长度: " << longestCommonSubsequence(text1, text2) << endl;
    
    text1 = "abc";
    text2 = "def";
    cout << "最长公共子序列长度: " << longestCommonSubsequence(text1, text2) << endl;
    
    return 0;
}
```

## 三、算法复杂度分析

### 1. 时间复杂度

| 算法 | 最好情况 | 平均情况 | 最坏情况 |
|------|---------|---------|---------|
| 线性查找 | O(1) | O(n) | O(n) |
| 二分查找 | O(1) | O(log n) | O(log n) |
| 冒泡排序 | O(n) | O(n²) | O(n²) |
| 快速排序 | O(n log n) | O(n log n) | O(n²) |
| 归并排序 | O(n log n) | O(n log n) | O(n log n) |
| 堆排序 | O(n log n) | O(n log n) | O(n log n) |

### 2. 空间复杂度

| 数据结构/算法 | 空间复杂度 |
|---------------|-----------|
| 数组 | O(n) |
| 链表 | O(n) |
| 栈 | O(n) |
| 队列 | O(n) |
| 二叉树 | O(n) |
| 快速排序 | O(log n) - O(n) |
| 归并排序 | O(n) |
| 深度优先搜索 | O(n) |
| 广度优先搜索 | O(n) |

## 四、高频笔试题目（ACM模式）

### 1. 两数之和

**题目描述**：给定一个整数数组 `nums` 和一个目标值 `target`，请你在该数组中找出和为目标值的那两个整数，并返回它们的数组下标。

**输入**：
```
3 6
2 7 11
```

**输出**：
```
0 1
```

**代码**：

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> hashMap;
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if (hashMap.find(complement) != hashMap.end()) {
            return {hashMap[complement], i};
        }
        hashMap[nums[i]] = i;
    }
    return {};
}

int main() {
    int n, target;
    cin >> n >> target;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    vector<int> result = twoSum(nums, target);
    cout << result[0] << " " << result[1] << endl;
    return 0;
}
```

### 2. 反转链表

**题目描述**：反转一个单链表。

**输入**：
```
5
1 2 3 4 5
```

**输出**：
```
5 4 3 2 1
```

**代码**：

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

void printList(ListNode* head) {
    ListNode* curr = head;
    while (curr) {
        cout << curr->val << " ";
        curr = curr->next;
    }
    cout << endl;
}

int main() {
    int n;
    cin >> n;
    ListNode* dummy = new ListNode(0);
    ListNode* curr = dummy;
    for (int i = 0; i < n; i++) {
        int val;
        cin >> val;
        curr->next = new ListNode(val);
        curr = curr->next;
    }
    ListNode* head = dummy->next;
    delete dummy;
    
    ListNode* reversed = reverseList(head);
    printList(reversed);
    
    // 释放内存
    while (reversed) {
        ListNode* temp = reversed;
        reversed = reversed->next;
        delete temp;
    }
    
    return 0;
}
```

### 3. 最大子数组和

**题目描述**：给定一个整数数组 `nums`，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

**输入**：
```
9
-2 1 -3 4 -1 2 1 -5 4
```

**输出**：
```
6
```

**代码**：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxSubArray(vector<int>& nums) {
    int currentSum = nums[0];
    int maxSum = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        currentSum = max(nums[i], currentSum + nums[i]);
        maxSum = max(maxSum, currentSum);
    }
    return maxSum;
}

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    cout << maxSubArray(nums) << endl;
    return 0;
}
```

### 4. 二叉树的层序遍历

**题目描述**：给你二叉树的根节点 `root`，返回其节点值的层序遍历（即逐层地，从左到右访问所有节点）。

**输入**：
```
3
1 2 3
2 4 5
3 -1 -1
4 -1 -1
5 -1 -1
```

**输出**：
```
1
2 3
4 5
```

**代码**：

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* buildTree(vector<vector<int>>& nodes, int rootVal) {
    if (rootVal == -1) return nullptr;
    TreeNode* root = new TreeNode(rootVal);
    for (auto& node : nodes) {
        if (node[0] == rootVal) {
            root->left = buildTree(nodes, node[1]);
            root->right = buildTree(nodes, node[2]);
            break;
        }
    }
    return root;
}

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> level;
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> nodes(n, vector<int>(3));
    for (int i = 0; i < n; i++) {
        cin >> nodes[i][0] >> nodes[i][1] >> nodes[i][2];
    }
    TreeNode* root = buildTree(nodes, nodes[0][0]);
    vector<vector<int>> result = levelOrder(root);
    for (auto& level : result) {
        for (int val : level) {
            cout << val << " ";
        }
        cout << endl;
    }
    return 0;
}
```

### 5. 快速排序

**题目描述**：使用快速排序对数组进行排序。

**输入**：
```
9
3 1 4 1 5 9 2 6 5
```

**输出**：
```
1 1 2 3 4 5 5 6 9
```

**代码**：

```cpp
#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (nums[j] <= pivot) {
            i++;
            swap(nums[i], nums[j]);
        }
    }
    swap(nums[i + 1], nums[right]);
    return i + 1;
}

void quickSort(vector<int>& nums, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(nums, left, right);
        quickSort(nums, left, pivotIndex - 1);
        quickSort(nums, pivotIndex + 1, right);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    quickSort(nums, 0, nums.size() - 1);
    for (int num : nums) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}
```

## 五、实战技巧

### 1. 如何分析问题

1. **理解问题**：仔细阅读题目，明确输入输出要求
2. **寻找规律**：分析示例，找出问题的内在规律
3. **选择算法**：根据问题特点选择合适的算法
4. **设计数据结构**：选择合适的数据结构来存储和处理数据
5. **编写代码**：实现算法，注意边界情况
6. **测试验证**：使用测试用例验证代码正确性

### 2. 如何优化代码性能

1. **时间优化**：
   - 选择更高效的算法
   - 减少时间复杂度
   - 避免不必要的计算

2. **空间优化**：
   - 减少内存使用
   - 使用原地算法
   - 合理使用缓存

3. **代码优化**：
   - 减少函数调用开销
   - 避免重复计算
   - 使用合适的STL容器

### 3. 常见错误类型

1. **边界条件**：未考虑空数组、单元素数组等情况
2. **内存泄漏**：未释放动态分配的内存
3. **数组越界**：访问超出数组范围的元素
4. **逻辑错误**：算法逻辑不正确
5. **性能问题**：算法复杂度太高，导致超时

### 4. 调试技巧

1. **打印调试**：在关键位置打印变量值
2. **断点调试**：使用IDE的断点功能逐步调试
3. **测试用例**：使用多种测试用例验证代码
4. **代码审查**：仔细检查代码逻辑

## 六、总结

算法与数据结构是计算机科学的基础，也是校招笔试面试的重点。通过本文档的学习，您应该能够：

1. **掌握常见数据结构**：链表、栈、队列、二叉树等的实现与应用
2. **理解经典算法**：排序、搜索、动态规划等算法的原理与实现
3. **分析算法复杂度**：评估算法的时间和空间复杂度
4. **解决高频笔试题目**：掌握ACM模式的编程题解法
5. **应用实战技巧**：提高代码质量和性能

在准备校招笔试面试时，建议多做练习，熟悉各种算法和数据结构的应用场景，培养解决问题的能力。通过系统的学习和实践，您一定能够在算法与数据结构方面取得优异的成绩。