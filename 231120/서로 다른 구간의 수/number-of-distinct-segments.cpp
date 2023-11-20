#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
using namespace std;

int n;
vector<pair<int, int>> arr;
set<tuple<int, int, int>> points;

void input() {
    cin >> n;
    arr = vector<pair<int, int>>(n);
    for (auto &&i : arr)
        cin >> i.first >> i.second;

    // (시작점, 1, idx), (끝점, -1, idx)
    for (int i = 0; i < n; i++) {
        points.insert({arr[i].first, 1, i});
        points.insert({arr[i].second, -1, i});
    }
}

int main() {
    input();

    // 시작점을 지나친다면 insert, 끝점을 지나친다면 erase
    int ans = 0, a, b, idx;
    set<int> uset;
    for (auto &&i : points) {
        tie(a, b, idx) = i;
        if (b == 1)
            uset.insert(idx);
        else
            uset.erase(uset.find(idx));
        ans += uset.empty();
    }
    cout << ans;
    return 0;
}