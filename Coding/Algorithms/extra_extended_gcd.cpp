/*
 * 手撕：扩展欧几里得
 *
 * 输入：一行 a b
 * 输出：gcd x y（满足 a*x+b*y=gcd）
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法
// ============================================================================
long long extgcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long a, b;
    if (!(cin >> a >> b)) return 0;
    long long x, y;
    long long g = extgcd(a, b, x, y);
    cout << g << ' ' << x << ' ' << y << '\n';
    return 0;
}
