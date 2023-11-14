#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;

int n;
vector<char> arr;
// 바위, 가위, 보에 대한 LR 부분합
vector<int> L[3], R[3];

void input() {
    cin >> n;
    arr = vector<char>(n);
    for (auto &&i : arr)
        cin >> i;

    unordered_map<char, int> tmp;
    tmp['H'] = 2, tmp['S'] = 0, tmp['P'] = 1;
    int sum_L[3] = {0}, sum_R[3] = {0};
    for (int i = 0; i < 3; i++)
        L[i] = R[i] = vector<int>(n, 0);
    for (int i = 0; i < n; i++) {
        sum_L[tmp[arr[i]]]++, sum_R[tmp[arr[n - 1 - i]]]++;
        for (int j = 0; j < 3; j++)
            L[j][i] = sum_L[j], R[j][n - 1 - i] = sum_R[j];
    }
}

int main() {
    input();
    int ans = 0;
    pair<int, int> c_arr[6] = {{0, 1}, {1, 0}, {0, 2}, {2, 0}, {1, 2}, {2, 1}};

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < n - 1; j++)
            ans = max(ans, L[c_arr[i].first][j] + R[c_arr[i].second][j + 1]);
    cout << ans;
    return 0;
}