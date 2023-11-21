#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#define END use_set.end()
using namespace std;

int n;
vector<pair<int, int>> arr;
set<tuple<int, int, int>> points;
// "대기 중"인 computer 목록
priority_queue<int, vector<int>, greater<int>> computers;

void input() {
    cin >> n;
    arr = vector<pair<int, int>>(n);
    for (auto &&i : arr)
        cin >> i.first >> i.second;

    for (int i = 0; i < n; i++) {
        computers.push(i + 1);
        points.insert({arr[i].first, 1, i});
        points.insert({arr[i].second, -1, i});
    }
}

vector<int> solution() {
    vector<int> user_arr(n, 0);
    int a, b, idx;
    for (auto &&i : points) {
        tie(a, b, idx) = i;
        // 사용 시작할 때 -> "대기 중"인 목록에서 제거
        if (b == 1) {
            user_arr[idx] = computers.top();
            computers.pop();
        }
        // 사용 끝낼 때 -> "대기 중"인 목록에 추가
        else
            computers.push(user_arr[idx]);
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