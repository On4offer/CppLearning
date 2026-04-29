/*
 * STL Demo：string、pair、tuple、bitset
 */
#include <bits/stdc++.h>
using namespace std;

void demo_string() {
    cout << "=== string ===\n";
    string s = "hello";
    s += " world";
    cout << s << " len=" << s.size() << "\n";
    cout << "substr(0,5)=" << s.substr(0, 5) << "\n";
    auto p = s.find("world");
    if (p != string::npos) cout << "find world at " << p << "\n";

    string num = "123";
    int v = stoi(num);
    cout << "stoi=" << v << "\n";

    stringstream ss("1 2 3");
    int a, b, c;
    ss >> a >> b >> c;
    cout << "sscanf style: " << a << b << c << "\n";
}

void demo_pair_tuple() {
    cout << "=== pair / tuple ===\n";
    pair<int, string> p = {1, "one"};
    cout << p.first << " " << p.second << "\n";
    cout << "make_pair: " << make_pair(2, string("two")).first << "\n";

    tuple<int, double, char> t = make_tuple(1, 2.5, 'x');
    cout << get<0>(t) << " " << get<1>(t) << "\n";
    int x;
    double y;
    char z;
    tie(x, y, z) = t;
    cout << "tie: " << x << " " << y << " " << z << "\n";

    // C++17 结构化绑定
    auto [xa, ya, za] = t;
    cout << "structured: " << xa << ya << za << "\n";
}

void demo_bitset() {
    cout << "=== bitset ===\n";
    bitset<8> bs(string("10101010"));
    cout << "bs=" << bs << " count=" << bs.count() << "\n";
    bs.flip();
    cout << "after flip: " << bs << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    demo_string();
    demo_pair_tuple();
    demo_bitset();
    return 0;
}
