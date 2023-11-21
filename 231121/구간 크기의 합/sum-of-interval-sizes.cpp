#include <iostream>
#include <queue>
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

    for (int i = 0; i < n; i++) {
        points.insert({arr[i].first, 1, i});
        points.insert({arr[i].second, -1, i});
    }
}

int main() {
    input();
    unordered_set<int> us;
    int ans = 0, start = 0, a, b, idx;
    
    for (auto &&i : points) {
        tie(a, b, idx) = i;
        if (b == 1) {
            us.insert(idx);
            if (us.size() == 1)
                start = a;
        } else {
            us.erase(idx);
            if (us.empty())
                ans += a - start;
        }
    }
    cout << ans;
    return 0;
}