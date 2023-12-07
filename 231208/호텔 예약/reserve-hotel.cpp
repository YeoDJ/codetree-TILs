#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_set>
using namespace std;

int n;
set<tuple<int, int, int>> arr;

void input() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        arr.insert({a, 1, i});
        arr.insert({b, -1, i});
    }
}

int main() {
    input();
    unordered_set<int> us;
    int a, b, idx, cnt = 0;

    for (auto &&i : arr) {
        tie(a, b, idx) = i;
        if (b == 1)
            us.insert(idx);
        else
            us.erase(idx);
        cnt += us.empty();
    }
    cout << cnt;
    return 0;
}