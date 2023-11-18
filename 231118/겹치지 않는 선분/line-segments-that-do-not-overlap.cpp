#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

int n;
vector<pair<int, int>> arr;
vector<int> L, R;

void input() {
    cin >> n;
    arr = vector<pair<int, int>>(n);
    L = R = vector<int>(n, 0);
    for (auto &&i : arr)
        cin >> i.first >> i.second;
    sort(arr.begin(), arr.end());

    L[0] = arr[0].second, R[n - 1] = arr[n - 1].second;
    for (int i = 1; i < n; i++) {
        L[i] = max(L[i - 1], arr[i].second);
        R[n - 1 - i] = min(R[n - i], arr[n - 1 - i].second);
    }
}

int main() {
    input();
    int ans = 0;
    for (int i = 1; i < n; i++)
        ans += L[i] != L[i - 1] || R[n - 1 - i] != R[n - i];
    cout << ans;
    return 0;
}