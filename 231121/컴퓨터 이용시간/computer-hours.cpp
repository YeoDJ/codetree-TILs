#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>
#define END use_set.end()
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

vector<int> solution() {
    vector<int> user_arr(n, 0);
    set<int> use_set;
    int com = 0, a, b, idx;

    for (auto &&i : points) {
        tie(a, b, idx) = i;
        if (b == 1) {
            auto it1 = use_set.find(com);
            auto it2 = --it1;
            auto it3 = END;
            it1++, it3--;
            com = (use_set.empty() || (it2 == END && *it1 > 1)) ? 1 : (*it1 - *it2 > 1) ? *it2 + 1 : *it3 + 1;
            use_set.insert(com);
            user_arr[idx] = com;
        } else
            use_set.erase(user_arr[idx]);
    }
    return user_arr;
}

int main() {
    input();
    vector<int> user_arr = solution();
    for (auto &&i : user_arr)
        cout << i << ' ';
    return 0;
}