/*
 * 手撕：埃氏筛素数
 *
 * 输入：一行 n
 * 输出：素数个数与所有素数
 */
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 核心方法
// ============================================================================
vector<int> sievePrimes(int n) {
    if (n < 2) return {};
    vector<char> isComp(n + 1, 0);
    for (int i = 2; i * i <= n; ++i) {
        if (isComp[i]) continue;
        for (int j = i * i; j <= n; j += i) isComp[j] = 1;
    }
    vector<int> primes;
    for (int i = 2; i <= n; ++i)
        if (!isComp[i]) primes.push_back(i);
    return primes;
}

// ============================================================================
// ACM：读入 → 调用 → 输出
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    auto primes = sievePrimes(n);
    cout << primes.size() << '\n';
    for (size_t i = 0; i < primes.size(); ++i) {
        if (i) cout << ' ';
        cout << primes[i];
    }
    cout << '\n';
    return 0;
}
