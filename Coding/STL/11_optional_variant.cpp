/*
 * STL Demo：C++17 optional / variant / string_view（轻量可选值与多类型安全联合体）
 * 编译需 -std=c++17
 */
#include <bits/stdc++.h>
using namespace std;

optional<int> parsePositive(const string& s) {
    try {
        int v = stoi(s);
        if (v > 0) return v;
    } catch (...) {
    }
    return nullopt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== optional ===\n";
    auto o1 = parsePositive("12");
    if (o1) cout << "value=" << *o1 << " or " << o1.value() << "\n";
    auto o2 = parsePositive("-1");
    cout << "has_value=" << o2.has_value() << "\n";

    cout << "=== variant（类型安全 union，需知道可能类型集合）===\n";
    variant<int, double, string> v = 1;
    v = 3.14;
    v = string("hello");
    cout << "index=" << v.index() << " holds string? " << holds_alternative<string>(v) << "\n";
    cout << get<string>(v) << "\n";

    cout << "=== string_view（不拥有字符，避免拷贝子串）===\n";
    string s = "hello world";
    string_view sv = s;
    string_view part = sv.substr(0, 5);
    cout << part << " len=" << part.size() << "\n";

    return 0;
}
