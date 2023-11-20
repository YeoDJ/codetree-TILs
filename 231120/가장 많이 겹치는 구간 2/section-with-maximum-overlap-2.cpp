#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
using namespace std;

int n;
vector<pair<int, int>> arr;
set<pair<int, int>> points;

void input() {
    cin >> n;
    arr = vector<pair<int, int>>(n);
    for (auto &&i : arr)
        cin >> i.first >> i.second;

    for (auto &&i : arr) {
        points.insert({i.first, 1});
        points.insert({i.second, -1});
    }
}

int main() {
    input();
    int ans = 0, cnt = 0;
    for (auto &&i : points) {
        cnt += i.second;
        ans = max(ans, cnt);
    }
    cout << ans;
    return 0;
}