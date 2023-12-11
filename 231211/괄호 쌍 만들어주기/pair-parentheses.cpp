#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
using namespace std;

typedef unsigned long long ull;
int n;
string str;
vector<ull> R;

void input() {
    cin >> str;
    n = str.size();
    R = vector<ull>(n, 0);

    int cnt = str[n - 1] == ')';
    for (int i = n - 2; i >= 0; i--) {
        cnt = (str[i] == ')') ? cnt + 1 : 0;
        R[i] = (cnt >= 2) ? R[i + 1] + 1 : R[i + 1];
    }
}

int main() {
    input();
    ull cnt = str[0] == '(', ans = 0;
    for (int i = 1; i < n; i++) {
        cnt = (str[i] == '(') ? cnt + 1 : 0;
        ans += (cnt >= 2) ? R[i] : 0;
    }
    cout << ans;
    return 0;
}