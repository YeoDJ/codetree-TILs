#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>
using namespace std;

int n, k;
vector<pair<int, int>> arr;
set<tuple<int, int, int>> points;

void input() {
    cin >> n >> k;

    int start = 0, end;
    char ch;
    for (int i = 0; i < n; i++) {
        cin >> end >> ch;
        end = (ch == 'L') ? start - end : start + end;
        arr.push_back({start, end});
        start = end;
        if (arr[i].first > arr[i].second)
            swap(arr[i].first, arr[i].second);
    }

    for (int i = 0; i < n; i++) {
        points.insert({arr[i].first, 1, i});
        points.insert({arr[i].second, -1, i});
    }
}

int main() {
    input();
    int ans = 0, start = 0, a, b, idx;
    unordered_set<int> uset;

    for (auto &&i : points) {
        tie(a, b, idx) = i;
        if (b == 1) {
            uset.insert(idx);
            if (uset.size() == k)
                start = a;
        } else {
            uset.erase(idx);
            if (uset.size() == k - 1)
                ans += a - start;
        }
    }
    cout << ans;
    return 0;
}