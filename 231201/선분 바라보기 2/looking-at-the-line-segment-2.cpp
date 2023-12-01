#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_set>
using namespace std;

int n;
vector<tuple<int, int, int, int>> points;

void input() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        int y, a, b;
        cin >> y >> a >> b;
        tuple<int, int, int, int> t1(a, y, 1, i), t2(b, y, -1, i);
        points.push_back(t1), points.push_back(t2);
    }
    sort(points.begin(), points.end());
}

int main() {
    input();

    int ans = 0, y, x, p, idx, pre_y = INT32_MAX;
    set<int> y_arr;
    for (auto &&i : points) {
        tie(x, y, p, idx) = i;
        if (p == 1)
            y_arr.insert(y);
        else
            y_arr.erase(y);

        y = *y_arr.begin();
        if (!y_arr.empty() && pre_y != y) {
            pre_y = y;
            ans++;
        }
    }
    cout << ans;
    return 0;
}