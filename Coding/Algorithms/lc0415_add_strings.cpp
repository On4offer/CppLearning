/*
 * 415. 字符串相加
 * 两个非负整数的十进制字符串表示，返回其和（字符串，无前导零，除 "0"）。
 *
 * 解法：模拟竖式加法，从低位到高位，维护进位 carry。
 * 时间 O(max(n,m))，空间 O(1) 除结果字符串。
 *
 * 输入：两行，分别为 num1、num2（仅数字字符）。
 * 输出：一行，和。
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法（题号 415）
// ============================================================================
string addStrings(string num1, string num2) {
    int i = (int)num1.size() - 1, j = (int)num2.size() - 1, carry = 0;
    string out;
    while (i >= 0 || j >= 0 || carry) {
        int x = i >= 0 ? num1[i--] - '0' : 0;
        int y = j >= 0 ? num2[j--] - '0' : 0;
        int s = x + y + carry;
        out.push_back(char('0' + (s % 10)));
        carry = s / 10;
    }
    reverse(out.begin(), out.end());
    return out;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string a, b;
    if (!(cin >> a >> b)) return 0;
    cout << addStrings(a, b) << '\n';
    return 0;
}
